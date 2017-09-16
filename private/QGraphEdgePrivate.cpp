/*!
   \file QGraphEdgePrivate.cpp
   \author Gregory Schultz <gregory.schultz@embarqmail.com>

   \section LICENSE
   This file is part of the Open|SpeedShop Graphical User Interface
   Copyright (C) 2010-2017 Argo Navis Technologies, LLC

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as published by the
   Free Software Foundation; either version 2.1 of the License, or (at your
   option) any later version.

   This library is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
   for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this library; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "QGraphEdgePrivate.h"

#include "QGraphCanvas.h"
#include "QGraphCanvasPrivate.h"
#include "QGraphNodePrivate.h"

#include "common/argonavis-lib-config.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QFontMetricsF>
#include <QPainter>

/**
 * @brief QGraphEdgePrivate::QGraphEdgePrivate
 * @param name - the edge label
 * @param canvas - the QGraphCanvasPrivate instance of the QGraphCanvas to which this edge is being added
 * @param head - the QGraphNodePrivate instance in which this edge instance in the out-edge
 * @param tail - the QGraphNodePrivate instance in which this edge instance in the in-edge
 * @param parent - the QGraphEdge instance being constructed
 *
 * The private implementation constructor for the QGraphEdge being constructed.
 */
QGraphEdgePrivate::QGraphEdgePrivate(const QString& name, QGraphCanvasPrivate* canvas, QGraphNodePrivate* head, QGraphNodePrivate* tail, QGraphEdge *parent)
    : q_ptr( parent )
{
    m_edge = agedge( canvas->graph(), head->node(), tail->node(), name.toLocal8Bit().data(), TRUE );

    setAttribute( QStringLiteral("label"), name.toLocal8Bit().data() );

    QDesktopWidget* desktopWidget = QApplication::desktop();
    m_logicalDpiY = ( desktopWidget ) ? desktopWidget->logicalDpiY() : 96.0;
}

/**
 * @brief QGraphEdgePrivate::QGraphEdgePrivate
 * @param edge - the libcgraph edge instance
 * @param parent - the QGraphEdge instance being created from this libcgraph edge instance
 *
 * The private implementation constructor for the QGraphEdge being constructed.
 */
QGraphEdgePrivate::QGraphEdgePrivate(Agedge_t *edge, QGraphEdge *parent)
    : q_ptr( parent )
    , m_edge( edge )
{

}

/**
 * @brief QGraphEdgePrivate::createNormalArrow
 * @param line - the final line segment in an edge to build the arrow on
 * @return - The lines forming the normal arrow type
 *
 * This function returns a polygon forming the normal arrow type.  To should be drawn using a brush to make it solid filled.
 * GraphViz arrow shapes are documented here: "http://graphviz.org/doc/info/arrows.html"
 */
QPolygonF QGraphEdgePrivate::createNormalArrow(const QLineF &line)
{
    const QLineF n = line.normalVector();

    const double SCALE_FACTOR = 0.5;

    const QTransform transform( SCALE_FACTOR, 0.0,          0.0,
                                0.0,          SCALE_FACTOR, 0.0,
                                0.0,          0.0,          1.0 );

    const QPointF translation = transform.map( QPointF( n.dx(), n.dy() ) );

    const QPointF firstAndLastPoint = line.p1() + translation;

    QPolygonF polygon;

    // add the three points of the arrow head
    polygon.append( firstAndLastPoint );
    polygon.append( line.p2() );
    polygon.append( line.p1() - translation );
    // close the polygon
    polygon.append( firstAndLastPoint );

    return polygon;
}

/**
 * @brief QGraphEdgePrivate::setAttribute
 * @param name - the name of the attribute to set
 * @param value - the value of the attribute to set
 *
 * Sets the named attribute of the edge to the new value.
 */
void QGraphEdgePrivate::setAttribute(const QString &name, const QString &value)
{
    const QString nullstr;
    agsafeset( m_edge, name.toLocal8Bit().data(), value.toLocal8Bit().data(), nullstr.toLocal8Bit().data() );
}

/**
 * @brief QGraphEdgePrivate::updateState
 *
 * This is the private implementation for the QGraphEdge::updateState() slot which is invoked to cause geometry and layout updates.
 */
void QGraphEdgePrivate::updateState()
{
    Q_Q(QGraphEdge);

    QDesktopWidget* desktopWidget = QApplication::desktop();
    m_logicalDpiY = ( desktopWidget ) ? desktopWidget->logicalDpiY() : 96.0;

    // first generate painter paths for the edge and edge label in global coordinates
    QPainterPath localPath = path();
    QPainterPath localLabelPath = labelPath( m_fontColor, m_fontSize, m_font );

    // generate painter paths for the edge arrows
    QPainterPath arrowPaths = arrowPath();

    // create a path containing both edge path and edge label path to determine bounding box of combined paths
    QPainterPath jointPath( localPath );
    jointPath.addPath( localLabelPath );

    // generate bounding box in global coordinates so that the center of this bounding bpx is used to set the edge's origin position in global coordinates
    QRectF boundingBox = jointPath.boundingRect();

    // set the graphics item origin position to the global bounding box center
    q->setPos( boundingBox.center() );

    // map the global bounding box to local graphics item coordinates
    m_boundingBox = q->mapRectFromScene( boundingBox );
    // map the edge painter path to local graphics item coordinates
    m_path = q->mapFromScene( localPath );
    // map the edge label painter path to local graphics item coordinates
    m_labelPath = q->mapFromScene( localLabelPath );
    // map the edge arrow painter path to the local graphics item coordinates
    m_arrowPath = q->mapFromScene( arrowPaths );
}

/**
 * @brief QGraphEdge::boundingRect
 * @return - the bounding rectangle of the graph edge instance
 *
 * Returns the proper bounding rectangle for the graph edge instance.  The bounding rectangle
 * is computed by the updateState() method as required.
 */
QRectF QGraphEdgePrivate::boundingRect() const
{
    return m_boundingBox;
}

/**
 * @brief QGraphEdgePrivate::height
 * @return - the height of the edge bounding box
 *
 * Returns the height of the edge bounding box.
 */
double QGraphEdgePrivate::height() const
{
    boxf bb = GD_bb( agraphof( m_edge ) );

    return ( bb.UR.y - bb.LL.y );
}

/**
 * @brief QGraphEdgePrivate::point
 * @param coord - the point coordinate in the Graphviz internal coordinate system
 * @return - coordinate in Qt coordinate system
 *
 * Transforms the point represented in the Graphviz internal coordinate system
 * to the Qt coordinate system.
 */
QPointF QGraphEdgePrivate::point(const pointf& coord) const
{
    // return coordinate in Qt coordinate system
    // - Qt coordinate system origin is on the top-left
    // - Graphviz internal coordinate system origin is on the botton-left
    // - thus just subtract graphviz y coordinate value from the graph height
    return QPointF( coord.x, height() - coord.y );
}

/**
 * @brief QGraphEdgePrivate::path
 * @return - the QPainterPath representation of the edge
 *
 * Uses the spline attribute state or the underlying libcgraph edge to return the QPainterPath representation.
 */
QPainterPath QGraphEdgePrivate::path() const
{
    QPainterPath path;

    // get the Cubic
    const splines* spline = ED_spl( m_edge );

    // process each Cubic Bezier Spline
    // NOTE: all points are expressed in the global coordinate system
    for( bezier* b = spline->list; b < spline->list + spline->size; b++ ) {
        if ( b->size % 3 != 1 )
            continue;  // Cubic Bezier Spline doesn't have the correct number of points
        // check if there is a beginning line segment
        if ( b->sflag ) {
            // draw line from starting point to first point in spline
            path.moveTo( point( b->sp ) );
            path.lineTo( point( b->list[0] ) );
            // create head arrow to build the proper outline for selection highlighting
            path.addPolygon( QGraphEdgePrivate::createNormalArrow( QLineF( point( b->list[0] ), point( b->sp ) ) ) );
        }
        else {
            // set the starting point of the spline
            path.moveTo( point( b->list[0] ) );
        }
        // add each segment of the Cubic Bezier Spline to the path
        for ( int i=1; i<b->size; i+=3 ) {
            path.cubicTo( point( b->list[i] ), point( b->list[i+1] ), point( b->list[i+2] ) );
        }
        // check if there is a ending line segment
        if ( b->eflag ) {
            // draw line from the last point in the spline to the ending point
            path.lineTo( point( b->ep ) );
            // create tail arrow to build the proper outline for selection highlighting
            path.addPolygon( QGraphEdgePrivate::createNormalArrow( QLineF( point( b->list[b->size-1] ), point( b->ep ) ) ) );
        }
    }

    return path;
}

/**
 * @brief QGraphEdgePrivate::arrowPath
 * @return - the QPainterPath representation of the edge head and tail arrows
 *
 * Uses the spline attribute state or the underlying libcgraph edge to return the QPainterPath representation.
 */
QPainterPath QGraphEdgePrivate::arrowPath() const
{
    QPainterPath path;

    // get the Cubic
    const splines* spline = ED_spl( m_edge );

    // process each Cubic Bezier Spline
    // NOTE: all points are expressed in the global coordinate system
    for( bezier* b = spline->list; b < spline->list + spline->size; b++ ) {
        if ( b->size % 3 != 1 )
            continue;  // Cubic Bezier Spline doesn't have the correct number of points
        // check if there is a beginning line segment
        if ( b->sflag ) {
            // position path at the first point in spline
            path.moveTo( point( b->list[0] ) );
            // create head arrow
            path.addPolygon( QGraphEdgePrivate::createNormalArrow( QLineF( point( b->list[0] ), point( b->sp ) ) ) );
        }
        // check if there is a ending line segment
        if ( b->eflag ) {
            // position path at the ending point
            path.moveTo( point( b->ep ) );
            // create tail arrow
            path.addPolygon( QGraphEdgePrivate::createNormalArrow( QLineF( point( b->list[b->size-1] ), point( b->ep ) ) ) );
        }
    }

    return path;
}

/**
 * @brief QGraphEdgePrivate::labelPath
 * @param fontcolor - returns the font color
 * @param fontsize - returns the font pixel size
 * @param font - returns the QFont object
 * @return - the QPainterPath representation of the edge label
 *
 * Uses the label attribute state or the underlying libcgraph edge to return the representation in Qt.
 */
QPainterPath QGraphEdgePrivate::labelPath(QColor& fontcolor, double& fontsize, QFont& font) const
{
    QPainterPath path;

    const textlabel_t* textlabel = ED_label( m_edge );

    if ( textlabel ) {
        // get label
        const QString label( textlabel->text );

        // center position of the label in node coordinates is either explicited defined in the label or set to the node origin
        // NOTE: all points are expressed in the global coordinate system
        const QPointF labelCenter = ( textlabel->set ) ? point( textlabel->pos ) : QPointF( 0.0, 0.0 );

        // get label font family and color
        const QColor fontcolorLocal( textlabel->fontcolor );
        QFont fontLocal( textlabel->fontname );
        // set pixel size by scaling point size in accordance to logical DPI Y of paint device
        fontLocal.setPixelSize( textlabel->fontsize * 72.0 / m_logicalDpiY );
        const QFontMetricsF fontMetrics( fontLocal );

        // get vertical alignment within label space rectangle
        int flags( Qt::AlignHCenter );  // center horizontally
        // determine vertical alignment and OR 'flags' variable with appropriate vertical alignment flag
        switch ( textlabel->valign ) {
        case 't': flags |= Qt::AlignTop; break;
        case 'b': flags |= Qt::AlignBottom; break;
        default: // include 'c'
            flags |= Qt::AlignVCenter; break;
        }

        // get label space rectangle
        const QRectF space( QPointF(0.0, 0.0), QPointF(textlabel->space.x, textlabel->space.y) );
        QRectF bbox = fontMetrics.boundingRect( space, flags, label );
        // place center of label space rectangle at specified label center coordinate
        bbox.moveCenter( labelCenter );

        // return the path to render the label (needs to be positioned at the label's baseline)
        path.addText( QPointF( bbox.x(), bbox.bottom() - fontMetrics.descent() ), fontLocal, label );

        fontcolor = fontcolorLocal;
        fontsize = textlabel->fontsize;
        font = fontLocal;
    }

    return path;
}

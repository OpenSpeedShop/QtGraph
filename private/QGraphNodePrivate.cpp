/*!
   \file QGraphNodePrivate.cpp
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

#include "QGraphNodePrivate.h"

#include "QGraphCanvas.h"
#include "QGraphCanvasPrivate.h"

#include "common/argonavis-lib-config.h"

#include <QFontMetricsF>
#include <QPainter>
#include <QDebug>


/**
 * @brief QGraphNodePrivate::QGraphNodePrivate
 * @param name - the node label
 * @param canvas - the QGraphCanvasPrivate instance of the QGraphCanvas to which this edge is being added
 * @param parent - the QGraphNode instance being constructed
 *
 * The private implementation constructor for the QGraphNode being constructed.
 */
QGraphNodePrivate::QGraphNodePrivate(const QString& name, QGraphCanvasPrivate* canvas, QGraphNode *parent)
    : q_ptr( parent )
    , m_fillColor( Qt::transparent )
    , m_penColor( Qt::black )
{
    m_node = agnode( canvas->graph(), name.toLocal8Bit().data(), TRUE );

    setAttribute( QStringLiteral("label"), name.toLocal8Bit().data() );
}

/**
 * @brief QGraphNodePrivate::QGraphNodePrivate
 * @param node - the libcgraph node instance
 * @param parent - the QGraphNode instance being created from this libcgraph node instance
 *
 * The private implementation constructor for the QGraphEdge being constructed.
 */
QGraphNodePrivate::QGraphNodePrivate(Agnode_t* node, QGraphNode *parent)
    : q_ptr( parent )
    , m_node( node )
    , m_fillColor( Qt::transparent )
    , m_penColor( Qt::black )
{

}

/**
 * @brief QGraphNodePrivate::node
 * @return - the underlying libcgraph node instance
 *
 * Returns the underlying libcgraph node instance.
 */
Agnode_t* QGraphNodePrivate::node() const
{
    return m_node;
}

/**
 * @brief QGraphNodePrivate::setAttribute
 * @param name - the name of the attribute to set
 * @param value - the value of the attribute to set
 *
 * Sets the named attribute of the node to the new value.
 */
void QGraphNodePrivate::setAttribute(const QString &name, const QString &value)
{
    const QString nullstr;
    agsafeset( m_node, name.toLocal8Bit().data(), value.toLocal8Bit().data(), nullstr.toLocal8Bit().data() );
}

/**
 * @brief QGraphNodePrivate::attribute
 * @param name - the name of the attribute
 * @return - the attribute value
 *
 * Gets the value of the named attribute of the node.
 */
QVariant QGraphNodePrivate::attribute(const QString &name) const
{
    return QVariant( agget( m_node, name.toLocal8Bit().data() ) );
}

/**
 * @brief QGraphNodePrivate::updateState
 *
 * This is the private implementation for the QGraphNode::updateState() slot which is invoked to cause geometry and layout updates.
 */
void QGraphNodePrivate::updateState()
{
    Q_Q(QGraphNode);
    q->setPos( point( ND_coord( m_node ) ) );

    // initialize default fill color
    QString fillColorStr = attribute("color").toString();
    if ( ! fillColorStr.isEmpty() ) {
        m_penColor = QColor( fillColorStr.toLocal8Bit().data() );
    }

    m_styles = style( attribute("style").toString() );

    foreach( Styles style, m_styles ) {
        switch( style ) {
        case Filled: {
            QString fillColorStr = attribute("fillcolor").toString();
            if ( fillColorStr.isEmpty() ) {
                const QString colorStr = attribute("color").toString();
                if ( ! colorStr.isEmpty() ) {
                    m_fillColor = QColor( colorStr.toLocal8Bit().data() );
                }
            }
            else {
                m_fillColor = QColor( fillColorStr.toLocal8Bit().data() );
            }
            break;
        }
        default:
            break;
        }
    }

    m_path = path( m_node );

    m_boundingBox = m_path.boundingRect();
}

/**
 * @brief QGraphNodePrivate::boundingRect
 * @return - the bounding rectangle of the graph node instance
 *
 * Returns the proper bounding rectangle for the graph node instance.  The bounding rectangle
 * is computed by the updateState() method as required.
 */
QRectF QGraphNodePrivate::boundingRect() const
{
    Q_Q(const QGraphNode);

    if ( q->isSelected() )
        return m_boundingBox.adjusted( -1.0, -1.0, 1.0, 1.0 );
    else
        return m_boundingBox;
}

/**
 * @brief QGraphEdgePrivate::point
 * @param coord - the point coordinate in the Graphviz internal coordinate system
 * @return - coordinate in Qt coordinate system
 *
 * Transforms the point represented in the Graphviz internal coordinate system
 * to the Qt coordinate system.
 */
QPointF QGraphNodePrivate::point(const pointf& coord) const
{
    // return coordinate in Qt coordinate system
    // - Qt coordinate system origin is on the top-left
    // - Graphviz internal coordinate system origin is on the botton-left
    // - thus just subtract graphviz y coordinate value from the graph height
    return QPointF( coord.x, height() - coord.y );
}

/**
 * @brief QGraphNodePrivate::style
 * @param str - the Graphviz edge line styles string value
 * @return - the set of internal style enums parsed from the input string
 *
 * Parses a comma-delimited string of edge line styles and returns a set of
 * internal style enums.
 */
QSet<QGraphNodePrivate::Styles> QGraphNodePrivate::style(const QString &str) const
{
    QSet<QGraphNodePrivate::Styles> styles;

    QStringList styleStrs = str.split( ',', QString::SkipEmptyParts );

    foreach( const QString& style, styleStrs ) {
        if ( style == "filled" )
            styles.insert( Filled );
        else if ( style == "invisible" )
            styles.insert( Invisible );
        else if ( style == "diagonals" )
            styles.insert( Diagonals );
        else if ( style == "rounded" )
            styles.insert( Rounded );
        else if ( style == "dashed" )
            styles.insert( Dashed );
        else if ( style == "dotted" )
            styles.insert( Dotted );
        else if ( style == "solid" )
            styles.insert( Solid );
        else if ( style == "bold" )
            styles.insert( Bold );
    }

    return styles;
}

/**
 * @brief QGraphEdgePrivate::height
 * @return - the height of the node bounding box
 *
 * Returns the height of the node bounding box.
 */
double QGraphNodePrivate::height() const
{
    const boxf bb = GD_bb( agraphof( m_node ) );

    return ( bb.UR.y - bb.LL.y );
}

/**
 * @brief QGraphNodePrivate::polygon
 * @param poly - a pointer to the Graphviz polygon instance
 * @param close - whether a closed polygon should be constructed
 * @return - a list of QPolygonF objects constructed from the Graphviz polygon instance
 *
 * Parses the Graphviz polygon instance and builds multiple QPolygonF instances returned as a QList.
 */
QList< QPolygonF > QGraphNodePrivate::polygon(const polygon_t *poly, const bool close) const
{
    QList< QPolygonF > polygons;

    // in geometry normal definition of polygon is to require a minimum of three sides but here for it may mean a bounding box defined using two points (sides)
    if ( poly->sides > 0 ) {
        pointf* firstVertix = poly->vertices;

        // vertix coordinates are relative to the polygon center and this matches the QGrahicsItem local coordinate system exactly
        for ( int i=0; i<poly->peripheries; i++ ) {
            QPolygonF polygon;
            pointf* vertix;
            for ( vertix = firstVertix; vertix < firstVertix + poly->sides; vertix++ ) {
                polygon.append( QPointF( vertix->x, vertix->y ) );
            }
            if ( close ) {
                // make this a closed polygon
                polygon.append( polygon.first() );
            }
            firstVertix = vertix;
            polygons.push_back( polygon );
        }

    }

    return polygons;
}

/**
 * @brief QGraphNodePrivate::path
 * @param n - the libcgraph node instance
 * @return - returns a QPainterPath representation of the node shape
 *
 * Constructs and returns a QPainterPath representation of the node shape.
 */
QPainterPath QGraphNodePrivate::path(Agnode_t* n) const
{
    // define list of supported shapes for the types of geometry that can be added to a QPainterPath (polygon and ellipse)
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
    static QStringList SHAPE_POLYGONS_TYPES{ "rectangle", "box", "square", "polygon", "diamond", "star", "rect", "triangle", "diamond",
                                             "trapezium", "parallelogram", "house", "pentagon", "hexagon", "septagon", "octagon", "doubleoctagon",
                                             "tripleoctagon", "invtriangle", "invtrapezium", "invhouse", "Mdiamond", "Msquare", "note"
                                           };
    static QStringList SHAPE_ELLIPSE_TYPES{ "ellipse", "circle", "oval",  };
    static QStringList SHAPE_RECORD_TYPES{ "record", "Mrecord" };
#else
    static QStringList SHAPE_POLYGONS_TYPES = QStringList() << "rectangle" << "box" << "square" << "polygon" << "diamond" << "star" << "rect" << "triangle" << "diamond"
                                                            << "trapezium" << "parallelogram" << "house" << "pentagon" << "hexagon" << "septagon" << "octagon" << "doubleoctagon"
                                                               "tripleoctagon" <<"invtriangle" << "invtrapezium" << "invhouse" << "Mdiamond" << "Msquare" << "note";
    static QStringList SHAPE_ELLIPSE_TYPES = QStringList() << "ellipse" << "circle" << "oval";
    static QStringList SHAPE_RECORD_TYPES = QStringList() << "record" << "Mrecord";
#endif

    QPainterPath path;

    const shape_desc* desc = ND_shape( n );

    const QString shapeType( desc->name );

    if ( SHAPE_POLYGONS_TYPES.contains( shapeType ) ) {
        const polygon_t* poly = (polygon_t*)ND_shape_info( n );

        QList< QPolygonF > polys = polygon( poly, true );

        foreach( const QPolygonF poly, polys ) {
            path.addPolygon( poly );
        }
    }
    else if ( SHAPE_ELLIPSE_TYPES.contains( shapeType ) ) {
        const polygon_t* poly = (polygon_t*)ND_shape_info( n );
        QList< QPolygonF > polys = polygon( poly, true );
        if ( polys.size() > 0 ) {
            const QPolygonF ellipseBoundingRect = polygon( poly ).front();
            if ( 2 == ellipseBoundingRect.size() ) {
                path.addEllipse( QRectF( ellipseBoundingRect.first(), ellipseBoundingRect.last() ) );
            }
        }
    }
#if 0
    else if ( SHAPE_RECORD_TYPES.contains( shapeType ) ) {
        const field_t* fields = (field_t*)ND_shape_info( n );
    }
#endif
    else {
        qWarning() << "UNSUPPORTED: shape type: " << shapeType;
    }

    return path;
}

/**
 * @brief QGraphNodePrivate::drawLabel
 * @param painter - the scene painter instance to be used for drawing the background
 *
 * Helper method to render the label for the node on the scene canvas.
 */
void QGraphNodePrivate::drawLabel(QPainter* painter) const
{
    const textlabel_t* textlabel = ND_label( m_node );

    if ( textlabel ) {
        // get label
        const QString label( textlabel->text );

        // center position of the label in node coordinates is either explicited defined in the label or set to the node origin
        const QPointF labelCenter = ( textlabel->set ) ? point( textlabel->pos ) : QPointF( 0.0, 0.0 );

        // get label font family and color
        const QColor fontcolor( textlabel->fontcolor );
        QFont font( textlabel->fontname );
        // set pixel size by scaling point size in accordance to logical DPI Y of paint device
        font.setPixelSize( textlabel->fontsize * 72.0 / painter->device()->logicalDpiY() );
        const QFontMetricsF fontMetrics( font );

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

        // set the drawing pen and draw the label
        painter->setPen( fontcolor );
        painter->setFont( font );
        painter->drawText( bbox, flags, label );
    }
}

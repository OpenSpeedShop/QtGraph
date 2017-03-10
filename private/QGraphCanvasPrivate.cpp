/*!
   \file QGraphCanvasPrivate.cpp
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

#include "QGraphCanvasPrivate.h"

#include "common/argonavis-lib-config.h"

#include "QGraphCanvas.h"
#include "QGraphNode.h"
#include "QGraphEdge.h"

#include "graphviz/cgraph.h"
#include "graphviz/gvc.h"

#include <QDebug>


/**
 * @brief QGraphCanvasPrivate::QGraphCanvasPrivate
 * @param name - the name of the graph
 * @param graphAttributeSettings - a list of name/value pairs specifying graph attribute settings
 * @param nodeAttributeSettings - a list of name/value pairs specifying node attribute settings
 * @param edgeAttributeSettings - a list of name/value pairs specifying edge attribute settings
 * @param parent - the QGraphCanvas parent
 *
 * Constructs an QGraphCanvasPrivate instance of the given QGraphCanvas parent with no graph nodes or edges defined.
 */
QGraphCanvasPrivate::QGraphCanvasPrivate(const QString &name,
                                         const QGraphCanvas::NameValueList& graphAttributeSettings,
                                         const QGraphCanvas::NameValueList& nodeAttributeSettings,
                                         const QGraphCanvas::NameValueList& edgeAttributeSettings,
                                         QGraphCanvas *parent)
    : q_ptr( parent )
    , m_drawGrid( false )
{
    //  set up a graphviz context - and init graph - retaining old API
    m_gvc = gvContext();

    // open graph
    m_graph = agopen( name.toLocal8Bit().data(), Agdirected, NULL );

    // set default attributes
    setAttributes( graphAttributeSettings, nodeAttributeSettings, edgeAttributeSettings );
}

/**
 * @brief QGraphCanvasPrivate::QGraphCanvasPrivate
 * @param name - pointer to a DOT formatted character array in memory
 * @param graphAttributeSettings - a list of name/value pairs specifying graph attribute settings
 * @param nodeAttributeSettings - a list of name/value pairs specifying node attribute settings
 * @param edgeAttributeSettings - a list of name/value pairs specifying edge attribute settings
 * @param parent - the QGraphCanvas parent
 *
 * Constructs an QGraphCanvasPrivate instance of the given QGraphCanvas parent from a pointer to a DOT formatted
 * character array in memory.  The graph nodes and edges will be created automatically.
 */
QGraphCanvasPrivate::QGraphCanvasPrivate(const char *data,
                                         const QGraphCanvas::NameValueList& graphAttributeSettings,
                                         const QGraphCanvas::NameValueList& nodeAttributeSettings,
                                         const QGraphCanvas::NameValueList& edgeAttributeSettings,
                                         QGraphCanvas *parent)
    : q_ptr( parent )
    , m_drawGrid( false )
{
    //  set up a graphviz context - and init graph - retaining old API
    m_gvc = gvContext();

    m_graph = agmemread( data );

    if ( m_graph ) {
        for (Agnode_t* node = agfstnode(m_graph); node != NULL; node = agnxtnode(m_graph, node)) {
            q_ptr->addGraphNode( node );
            for (Agedge_t* edge = agfstout(m_graph, node); edge != NULL; edge = agnxtout(m_graph, edge)) {
                q_ptr->addGraphEdge( edge  );
            }
        }
    }

    // set default attributes
    setAttributes( graphAttributeSettings, nodeAttributeSettings, edgeAttributeSettings );
}

/**
 * @brief QGraphCanvasPrivate::graph
 * @return - the underlying libcgraph graph instance
 *
 * Returns the underlying libcgraph graph instance.
 */
Agraph_t* QGraphCanvasPrivate::graph() const
{
    return m_graph;
}

/**
 * @brief QGraphCanvasPrivate::setGraphAttribute
 * @param name - the name of the graph attribute to set
 * @param value - the value of the graph attribute to set
 *
 * Sets the default value of the named graph attribute.
 */
void QGraphCanvasPrivate::setGraphAttribute(const QString &name, const QString &value)
{
    if ( NULL == agattr( m_graph, AGRAPH, name.toLocal8Bit().data(), value.toLocal8Bit().data() ) ) {
        qCritical() << "ERROR: Unable to set graph attribute '" << name << "'";
    }
}

/**
 * @brief QGraphCanvasPrivate::setNodeAttribute
 * @param name - the name of the node attribute to set
 * @param value - the value of the node attribute to set
 *
 * Sets the default value of the named node attribute.
 */
void QGraphCanvasPrivate::setNodeAttribute(const QString &name, const QString &value)
{
    if ( NULL == agattr( m_graph, AGNODE, name.toLocal8Bit().data(), value.toLocal8Bit().data() ) ) {
        qCritical() << "ERROR: Unable to set node attribute '" << name << "'";
    }
}

/**
 * @brief QGraphCanvasPrivate::setEdgeAttribute
 * @param name - the name of the edge attribute to set
 * @param value - the value of the edge attribute to set
 *
 * Sets the default value of the named edge attribute.
 */
void QGraphCanvasPrivate::setEdgeAttribute(const QString &name, const QString &value)
{
    if ( NULL == agattr( m_graph, AGEDGE, name.toLocal8Bit().data(), value.toLocal8Bit().data() ) ) {
        qCritical() << "ERROR: Unable to set edge attribute '" << name << "'";
    }
}

/**
 * @brief QGraphCanvasPrivate::showGrid
 * @param shown - specifies whether the grid will be drawn on the graph canvas
 *
 * Sets whether the grid will be drawn on the graph canvas.
 */
void QGraphCanvasPrivate::showGrid(bool shown)
{
    m_drawGrid = shown;
}

/**
 * @brief QGraphCanvasPrivate::updateLayout
 *
 * Updates the layout of the graph canvas.
 */
void QGraphCanvasPrivate::updateLayout()
{
    if ( m_gvc && m_graph ) {
        if ( 0 == gvLayout( m_gvc, m_graph, "dot" ) ) {
            Q_Q(QGraphCanvas);

            emit q->layoutUpdated();
        }
    }
}

/**
 * @brief QGraphCanvasPrivate::setAttributes
 * @param graphAttributeSettings - a list of name/value pairs specifying graph attribute settings
 * @param nodeAttributeSettings - a list of name/value pairs specifying node attribute settings
 * @param edgeAttributeSettings - a list of name/value pairs specifying edge attribute settings
 *
 * Sets the default graph, node and edge attributes.
 */
void QGraphCanvasPrivate::setAttributes(const QGraphCanvas::NameValueList &graphAttributeSettings,
                                        const QGraphCanvas::NameValueList &nodeAttributeSettings,
                                        const QGraphCanvas::NameValueList &edgeAttributeSettings)
{
    // configure graph attributes
    foreach ( const QGraphCanvas::NameValuePair& nameValuePair, graphAttributeSettings ) {
        setGraphAttribute( nameValuePair.first, nameValuePair.second );
    }

    // configure node attributes
    foreach ( const QGraphCanvas::NameValuePair& nameValuePair, nodeAttributeSettings ) {
        setNodeAttribute( nameValuePair.first, nameValuePair.second );
    }

    // configure edge attributes
    foreach ( const QGraphCanvas::NameValuePair& nameValuePair, edgeAttributeSettings ) {
        setEdgeAttribute( nameValuePair.first, nameValuePair.second );
    }
}

/**
 * @brief QGraphCanvasPrivate::boundingRect
 * @return - the bounding rectangle of the graph canvas instance
 *
 * This method returns the bounding rectangle of the underlying libcgraph graph instance.
 */
QRectF QGraphCanvasPrivate::boundingBox() const
{
    QRectF sceneRect;

    // get bounding box of the graph
    boxf boundingBox = GD_bb( m_graph );

    // the Graphviz rectangle is in terms of lower-left and upper-right so set these attributes in a Qt QRectF class object
    sceneRect.setBottomLeft( QPointF( boundingBox.LL.x, boundingBox.LL.y ) );
    sceneRect.setTopRight( QPointF( boundingBox.UR.x, boundingBox.UR.y ) );

    // return the normalized rectangle - i.e. one with non-negative height and width
    sceneRect = sceneRect.normalized();

    return sceneRect;
}

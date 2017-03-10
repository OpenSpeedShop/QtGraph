/*!
   \file QGraphCanvas.cpp
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

#include "QGraphCanvas.h"
#include "QGraphCanvasPrivate.h"
#include "QGraphNode.h"
#include "QGraphEdge.h"

#include <QPainter>
#include <QDebug>


/**
 * @brief QGraphCanvas::QGraphCanvas
 * @param name - the name of the graph
 * @param graphAttributeSettings - a list of name/value pairs specifying graph attribute settings
 * @param nodeAttributeSettings - a list of name/value pairs specifying node attribute settings
 * @param edgeAttributeSettings - a list of name/value pairs specifying edge attribute settings
 * @param parent - the parent widget
 *
 * Constructs an QGraphCanvas instance of the given parent with no graph nodes or edges defined.
 */
QGraphCanvas::QGraphCanvas(const QString &name,
                           const NameValueList& graphAttributeSettings,
                           const NameValueList& nodeAttributeSettings,
                           const NameValueList& edgeAttributeSettings,
                           QObject *parent)
    : QGraphicsScene( parent )
    , d_ptr( new QGraphCanvasPrivate( name, graphAttributeSettings, nodeAttributeSettings, edgeAttributeSettings, this ) )
{
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
    connect( this, &QGraphCanvas::layoutUpdated, this, &QGraphCanvas::handleLayoutUdated );
#else
    connect( this, SIGNAL(layoutUpdated()), this, SLOT(handleLayoutUdated()) );
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
    connect( this, &QGraphCanvas::selectionChanged, this, &QGraphCanvas::handleSelectionChanged );
#else
    connect( this, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()) );
#endif
}

/**
 * @brief QGraphCanvas::QGraphCanvas
 * @param name - pointer to a DOT formatted character array in memory
 * @param graphAttributeSettings - a list of name/value pairs specifying graph attribute settings
 * @param nodeAttributeSettings - a list of name/value pairs specifying node attribute settings
 * @param edgeAttributeSettings - a list of name/value pairs specifying edge attribute settings
 * @param parent - the parent widget
 *
 * Constructs an QGraphCanvas instance of the given parent from a pointer to a DOT formatted
 * character array in memory.  The graph nodes and edges will be created automatically.
 */
QGraphCanvas::QGraphCanvas(const char *data,
                           const NameValueList& graphAttributeSettings,
                           const NameValueList& nodeAttributeSettings,
                           const NameValueList& edgeAttributeSettings,
                           QObject *parent)
    : QGraphicsScene( parent )
    , d_ptr( new QGraphCanvasPrivate( data, graphAttributeSettings, nodeAttributeSettings, edgeAttributeSettings, this ) )
{
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
    connect( this, &QGraphCanvas::layoutUpdated, this, &QGraphCanvas::handleLayoutUdated );
#else
    connect( this, SIGNAL(layoutUpdated()), this, SLOT(handleLayoutUdated()) );
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
    connect( this, &QGraphCanvas::selectionChanged, this, &QGraphCanvas::handleSelectionChanged );
#else
    connect( this, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()) );
#endif
}

/**
 * @brief QGraphCanvas::QGraphCanvas
 * @param name - the name of the graph
 * @param sceneRect - the defined bounding rectangle for the scene
 * @param graphAttributeSettings - a list of name/value pairs specifying graph attribute settings
 * @param nodeAttributeSettings - a list of name/value pairs specifying node attribute settings
 * @param edgeAttributeSettings - a list of name/value pairs specifying edge attribute settings
 * @param parent - the parent widget
 *
 * Constructs an QGraphCanvas instance of the given parent with no graph nodes or edges defined.
 */
QGraphCanvas::QGraphCanvas(const QString &name, const QRectF &sceneRect,
                           const NameValueList& graphAttributeSettings,
                           const NameValueList& nodeAttributeSettings,
                           const NameValueList& edgeAttributeSettings,
                           QObject *parent)
    : QGraphicsScene( sceneRect, parent )
    , d_ptr( new QGraphCanvasPrivate( name, graphAttributeSettings, nodeAttributeSettings, edgeAttributeSettings, this ) )
{
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
    connect( this, &QGraphCanvas::layoutUpdated, this, &QGraphCanvas::handleLayoutUdated );
#else
    connect( this, SIGNAL(layoutUpdated()), this, SLOT(handleLayoutUdated()) );
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
    connect( this, &QGraphCanvas::selectionChanged, this, &QGraphCanvas::handleSelectionChanged );
#else
    connect( this, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()) );
#endif
}

/**
 * @brief QGraphCanvas::QGraphCanvas
 * @param x - the top-left corner x-coordinate of the bounding rectangle for the scene
 * @param y - the top-left corner y-coordinate of the bounding rectangle for the scene
 * @param width - the width of the bounding rectangle for the scene
 * @param height - the height of the bounding rectangle for the scene
 * @param sceneRect - the defined bounding rectangle for the scene
 * @param graphAttributeSettings - a list of name/value pairs specifying graph attribute settings
 * @param nodeAttributeSettings - a list of name/value pairs specifying node attribute settings
 * @param edgeAttributeSettings - a list of name/value pairs specifying edge attribute settings
 * @param parent - the parent widget
 *
 * Constructs an QGraphCanvas instance of the given parent with no graph nodes or edges defined.
 */
QGraphCanvas::QGraphCanvas(const QString &name, qreal x, qreal y, qreal width, qreal height,
                           const NameValueList& graphAttributeSettings,
                           const NameValueList& nodeAttributeSettings,
                           const NameValueList& edgeAttributeSettings,
                           QObject *parent)
    : QGraphicsScene( x, y, width, height, parent )
    , d_ptr( new QGraphCanvasPrivate( name, graphAttributeSettings, nodeAttributeSettings, edgeAttributeSettings, this ) )
{
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
    connect( this, &QGraphCanvas::layoutUpdated, this, &QGraphCanvas::handleLayoutUdated );
#else
    connect( this, SIGNAL(layoutUpdated()), this, SLOT(handleLayoutUdated()) );
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
    connect( this, &QGraphCanvas::selectionChanged, this, &QGraphCanvas::handleSelectionChanged );
#else
    connect( this, SIGNAL(selectionChanged()), this, SLOT(handleSelectionChanged()) );
#endif
}

/**
 * @brief QGraphCanvas::~QGraphCanvas
 *
 * Destroys the QGraphCanvas instance.
 */
QGraphCanvas::~QGraphCanvas()
{
    Q_D(QGraphCanvas);
    delete d;
}

/**
 * @brief QGraphCanvas::updateLayout
 *
 * Updates the layout of the graph canvas.
 */
void QGraphCanvas::updateLayout()
{
    Q_D(QGraphCanvas);
    d->updateLayout();
}

/**
 * @brief QGraphCanvas::showGrid
 * @param shown - indicates whether the graph grid background will be shown
 *
 * Sets the state flag indicating whether the grid will be drawn on the canvas background.
 */
void QGraphCanvas::showGrid(bool shown)
{
    Q_D(QGraphCanvas);

    d->showGrid( shown );
}

/**
 * @brief QGraphCanvas::addGraphNode
 * @param node - the QGraphNode instance to add to the graph canvas
 *
 * Adds a new node instance to the graph.  Connects the QGraphCanvas::layoutUpdated signal
 * to a graph node handler for updating the node state.
 */
void QGraphCanvas::addGraphNode(QGraphNode *node)
{
    addItem( node );

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
    connect( this, &QGraphCanvas::layoutUpdated, node, &QGraphNode::updateState );
#else
    connect( this, SIGNAL(layoutUpdated()), node, SLOT(updateState()) );
#endif
}

/**
 * @brief QGraphCanvas::addGraphNode
 * @param edge - the QGraphEdge instance to add to the graph canvas
 *
 * Adds a new edge instance to the graph.  Connects the QGraphCanvas::layoutUpdated signal
 * to a graph edge handler for updating the edge state.
 */
void QGraphCanvas::addGraphEdge(QGraphEdge *edge)
{
    addItem( edge );

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
    connect( this, &QGraphCanvas::layoutUpdated, edge, &QGraphEdge::updateState );
#else
    connect( this, SIGNAL(layoutUpdated()), edge, SLOT(updateState()) );
#endif
}

/**
 * @brief QGraphCanvas::addItem
 * @param item - the QGraphicsItem to add to the scene
 *
 * Overrides the public QGraphicsScene::addItem method and makes it protected.  It is invoked
 * by the public QGraphCanvas::addGraphNode and QGraphCanvas::addGraphEdge methods.
 */
void QGraphCanvas::addItem(QGraphicsItem *item)
{
    QGraphicsScene::addItem( item );
}

/**
 * @brief QGraphCanvas::drawBackground
 * @param painter - the scene painter instance to be used for drawing the background
 * @param rect - the exposed rectangle within the scene
 *
 * Reimplements the QGraphicsScene::drawBackground method to draw a grid background.
 * All painting is done in scene coordinates.
 */
void QGraphCanvas::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_D(QGraphCanvas);

    if ( d->drawGrid() ) {
        painter->save();

        const QPen gridPen(qRgb(240, 240, 240));

        painter->setPen( gridPen );

        const QRectF srect = sceneRect();

        for (qreal x = srect.x(); x <= srect.width(); x+=10.0 ) {
            painter->drawLine( QLineF( QPointF(x,srect.y()), QPointF(x, srect.height()) ) );
        }

        for (qreal y = srect.y(); y <= srect.height(); y+=10.0) {
            painter->drawLine( QLineF( QPointF(srect.x(),y), QPointF(srect.width(),y) ) );
        }

        const QPen borderPen( QColor(Qt::gray), 1.5 );

        painter->setPen( borderPen );

        painter->drawRect( srect );

        painter->restore();
    }

    QGraphicsScene::drawBackground( painter, rect );
}

/**
 * @brief QGraphCanvas::addGraphNode
 * @param node - a libcgraph node to construct a QGraphNode instance
 *
 * Constructs a QGraphNode instance from a libcgraph node and adds to the graph.
 */
void QGraphCanvas::addGraphNode(void *node)
{
    addGraphNode( new QGraphNode( node ) );
}

/**
 * @brief QGraphCanvas::addGraphEdge
 * @param node - a libcgraph edge to construct a QGraphEdge instance
 *
 * Constructs a QGraphEdge instance from a libcgraph edge and adds to the graph.
 */
void QGraphCanvas::addGraphEdge(void *edge)
{
    addGraphEdge( new QGraphEdge( edge ) );
}

/**
 * @brief QGraphCanvas::handleLayoutUdated
 *
 * Handler for layoutUpdated() signal to set a new scene bounding box.
 */
void QGraphCanvas::handleLayoutUdated()
{
    Q_D(QGraphCanvas);
    setSceneRect( d->boundingBox() );
}

/**
 * @brief QGraphCanvas::handleSelectionChanged
 *
 * Handlers for QGraphicsScene::selectionChanged() signal emitted when the
 * selection of graph items in the scene changes.
 */
void QGraphCanvas::handleSelectionChanged()
{
    const QList<QGraphicsItem *> items = selectedItems();
    foreach( QGraphicsItem* item, items ) {
        if ( item->type() == QGraphNode::Type )
            emit graphNodeSelected( qgraphicsitem_cast<QGraphNode*>(item) );
        else if ( item->type() == QGraphEdge::Type )
            emit graphEdgeSelected( qgraphicsitem_cast<QGraphEdge*>(item) );
    }
}

/*!
   \file QGraphEdge.cpp
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

#include "QGraphEdge.h"
#include "QGraphEdgePrivate.h"

#include "QGraphCanvas.h"
#include "QGraphNode.h"

#include <QPainter>
#include <QGraphicsColorizeEffect>


/**
 * @brief QGraphEdge::QGraphEdge
 * @param name - the edge label
 * @param canvas - the QGraphCanvas instance in which the edge resides
 * @param head - the QGraphNode instance in which this edge instance in the out-edge
 * @param tail - the QGraphNode instance in which this edge instance in the in-edge
 * @param parent - the parent QGraphicsObject instance
 *
 * Constructs a selectable QGraphEdge instance.  Creates an underlying libcgraph edge instance attached to
 * the two libcgraph node instances specified by 'head' and 'tail'.
 */
QGraphEdge::QGraphEdge(const QString& name, QGraphCanvas* canvas, QGraphNode* head, QGraphNode* tail, QGraphicsItem *parent)
    : QGraphicsObject( parent )
    , d_ptr( new QGraphEdgePrivate(name, canvas->d_ptr, head->d_ptr, tail->d_ptr, this) )
{
    setZValue( 1.0 );
    setFlag( QGraphicsItem::ItemIsSelectable, true );
}

/**
 * @brief QGraphEdge::QGraphEdge
 * @param edge - the libcgraph edge instance
 * @param parent - the parent QGraphicsObject instance
 *
 * Constructs a selectable QGraphEdge instance directly from a libcgraph edge.  The attached head and tail nodes were
 * defined when the libcgraph edge instance was created.
 */
QGraphEdge::QGraphEdge(void *edge, QGraphicsItem *parent)
    : QGraphicsObject( parent )
    , d_ptr( new QGraphEdgePrivate((Agedge_t*) edge, this) )
{
    setZValue( 1.0 );
    setFlag( QGraphicsItem::ItemIsSelectable, true );
}

/**
 * @brief QGraphEdge::~QGraphEdge
 *
 * Destroys the QGraphEdge instance.
 */
QGraphEdge::~QGraphEdge()
{
    Q_D(QGraphEdge);
    delete d;
}

/**
 * @brief QGraphEdge::updateState
 *
 * This slot is invoked to cause geometry and layout updates.
 */
void QGraphEdge::updateState()
{
    Q_D(QGraphEdge);
    prepareGeometryChange();
    d->updateState();
}

/**
 * @brief QGraphEdge::boundingRect
 * @return - the bounding rectangle of the graph edge instance
 *
 * This implementation overrides the QGraphicsObject::boundingRect() method to return
 * the proper bounding rectangle for the graph edge instance.
 */
QRectF QGraphEdge::boundingRect() const
{
    Q_D(const QGraphEdge);
    return d->boundingRect();
}

/**
 * @brief QGraphEdge::itemChange
 * @param change - the parameter of the item that is changing
 * @param value - the new value of the parameter
 * @return - the final value of the parameter
 *
 * Reimplementation of the QGraphicsObject::itemChange virtual function used to notify instance
 * that some part of the item's state changed.  In particular, when the item is selected, this function
 * sets the graphics effect on the instance and when it is no longer selected removes the graphics effect.
 */
QVariant QGraphEdge::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if ( change == ItemSelectedHasChanged ) {
        bool selected = value.toBool();
        if ( selected )
            setGraphicsEffect( new QGraphicsColorizeEffect );  // ownership is passed to base class implementation
        else
            setGraphicsEffect( 0 );  // remove graphics effects
    }

    return QGraphicsObject::itemChange( change, value );
}

/**
 * @brief QGraphEdge::shape
 * @return - the painter path representing the outline of the edge
 *
 * Reimplementation of the QGraphicsObject::shape virtual function used to generate a QPainterPath
 * representation of the outline of the edge instance.
 */
QPainterPath QGraphEdge::shape() const
{
    Q_D(const QGraphEdge);
    // initialize a painter path stroker to generate an outline of the edge and edge label painter paths
    // this will make a slight larger area to make it easier for the suer to select edges
    QPainterPathStroker stroker;
    stroker.setWidth( 20 );
    // create a path containing both edge path and edge label bounding box
    QPainterPath jointPath( d->m_path );
    jointPath.addRect( d->m_labelPath.boundingRect() );
    // return the outline of the combined painter path
    return stroker.createStroke( jointPath );
}

/**
 * @brief QGraphEdge::paint
 * @param painter - the scene painter instance to be used for drawing the background
 * @param option - provides style options for the item, such as its state, exposed area and its level-of-detail hints
 * @param widget -it points to the widget that is being painted on (for cached painting this is NULL)
 *
 * Reimplementation of the QGraphicsObject::paint virtual function used to draw the edge on the graph canvas.
 */
void QGraphEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    Q_D(QGraphEdge);

    painter->save();

    QPen pPen( painter->pen() );

    if ( isSelected() ) {
        QPen pen( pPen );
        pen.setWidthF( pPen.widthF() * 2.0 );
        painter->setPen( pen );
    }

    painter->drawPath( d->m_path );

    if ( ! d->m_labelPath.isEmpty() ) {
        QPen pen( d->m_fontColor, pPen.widthF() / 4.0 );
        painter->setPen( pen );
        painter->setBrush( QBrush( d->m_fontColor, Qt::SolidPattern ) );
        painter->drawPath( d->m_labelPath );
    }

    painter->restore();
}

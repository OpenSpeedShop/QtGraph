/*!
   \file QGraphNode.cpp
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

#include "QGraphNode.h"
#include "QGraphNodePrivate.h"

#include "QGraphCanvas.h"

#include <QPainter>
#include <QGraphicsColorizeEffect>
#include <QDebug>


/**
 * @brief QGraphNode::QGraphNode
 * @param name - the node label
 * @param canvas - the QGraphCanvas instance in which the node resides
 * @param parent - the parent QGraphicsObject instance
 *
 * Constructs a selectable QGraphNode instance.  Creates the underlying libcgraph node instance.
 */
QGraphNode::QGraphNode(const QString& name, QGraphCanvas* canvas, QGraphicsItem *parent)
    : QGraphicsObject( parent )
    , d_ptr( new QGraphNodePrivate(name, canvas->d_ptr, this) )
{
    setZValue( 1.0 );
    setFlag( QGraphicsItem::ItemIsSelectable, true );
}

/**
 * @brief QGraphNode::QGraphNode
 * @param node - the libcgraph node instance
 * @param parent - the parent QGraphicsObject instance
 *
 * Constructs a selectable QGraphNode instance directly from a libcgraph node.
 */
QGraphNode::QGraphNode(void *node, QGraphicsItem *parent)
    : QGraphicsObject( parent )
    , d_ptr( new QGraphNodePrivate((Agnode_t*) node, this) )
{
    setZValue( 1.0 );
    setFlag( QGraphicsItem::ItemIsSelectable, true );
}

/**
 * @brief QGraphNode::~QGraphNode
 *
 * Destroys the QGraphNode instance.
 */
QGraphNode::~QGraphNode()
{
    Q_D(QGraphNode);
    delete d;
}

/**
 * @brief QGraphNode::itemChange
 * @param change - the parameter of the item that is changing
 * @param value - the new value of the parameter
 * @return - the final value of the parameter
 *
 * Reimplementation of the QGraphicsObject::itemChange virtual function used to notify instance
 * that some part of the item's state changed.  In particular, when the item is selected, this function
 * sets the graphics effect on the instance and when it is no longer selected removes the graphics effect.
 */
QVariant QGraphNode::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
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
 * @brief QGraphNode::shape
 * @return - the painter path representing the outline of the node
 *
 * Reimplementation of the QGraphicsObject::shape virtual function used to generate a QPainterPath
 * representation of the outline of the node instance.
 */
QPainterPath QGraphNode::shape() const
{
    Q_D(const QGraphNode);
    return d->m_path;
}

/**
 * @brief QGraphNode::setAttribute
 * @param name - the name of the attribute to set
 * @param value - the value of the attribute to set
 *
 * Sets the named attribute of the node to the new value.
 */
void QGraphNode::setAttribute(const QString &name, const QString &value)
{
    Q_D(QGraphNode);
    d->setAttribute( name, value );
}

/**
 * @brief QGraphNode::updateState
 *
 * This slot is invoked to cause geometry and layout updates.
 */
void QGraphNode::updateState()
{
    Q_D(QGraphNode);
    prepareGeometryChange();
    d->updateState();
}

/**
 * @brief QGraphNode::boundingRect
 * @return - the bounding rectangle of the graph node instance
 *
 * This implementation overrides the QGraphicsObject::boundingRect() method to return
 * the proper bounding rectangle for the graph node instance.
 */
QRectF QGraphNode::boundingRect() const
{
    Q_D(const QGraphNode);
    return d->boundingRect();
}

/**
 * @brief QGraphNode::paint
 * @param painter - the scene painter instance to be used for drawing the background
 * @param option - provides style options for the item, such as its state, exposed area and its level-of-detail hints
 * @param widget -it points to the widget that is being painted on (for cached painting this is NULL)
 *
 * Reimplementation of the QGraphicsObject::paint virtual function used to draw the node on the graph canvas.
 */
void QGraphNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    Q_D(QGraphNode);

    if ( d->m_styles.contains( QGraphNodePrivate::Invisible ) )
         return;

    painter->save();

    painter->setBrush( d->m_fillColor );
    painter->setPen ( d->m_penColor );

    painter->drawPath( d->m_path );

    // for now not rendered as painter path generated in updateState() but may have to change if
    // node labels can extend outside the node bounding rectangle
    d->drawLabel( painter );

    painter->restore();
}

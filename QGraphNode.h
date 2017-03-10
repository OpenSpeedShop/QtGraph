/*!
   \file QGraphNode.h
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

#ifndef QGRAPHNODE_H
#define QGRAPHNODE_H

#include <QGraphicsObject>

#include "argonavis-lib-config.h"
#include "qtgraphlibrary.h"

class QGraphCanvas;
class QGraphNodePrivate;


class QTGRAPH_EXPORT QGraphNode : public QGraphicsObject
{
    Q_OBJECT
    Q_DISABLE_COPY(QGraphNode)
    Q_DECLARE_PRIVATE(QGraphNode)

    friend class QGraphCanvas;
    friend class QGraphEdge;

public:

    explicit QGraphNode(const QString& name, QGraphCanvas* canvas, QGraphicsItem *parent = Q_NULLPTR);
    explicit QGraphNode(QGraphicsItem *parent = Q_NULLPTR) = delete;
    ~QGraphNode();

    enum { Type = UserType + 1 };

    int type() const { return Type; }

    void setAttribute(const QString &name, const QString &value);

public slots:

    void updateState();

protected:

    explicit QGraphNode(void* node, QGraphicsItem *parent = Q_NULLPTR);

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
    virtual QPainterPath shape() const Q_DECL_OVERRIDE;
    virtual QRectF boundingRect() const Q_DECL_OVERRIDE;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) Q_DECL_OVERRIDE;

protected:

    QGraphNodePrivate* const d_ptr;

};

#endif // QGRAPHNODE_H

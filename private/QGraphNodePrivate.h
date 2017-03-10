/*!
   \file QGraphNodePrivate.h
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

#ifndef QGRAPHNODEPRIVATE_H
#define QGRAPHNODEPRIVATE_H

#include "QGraphNode.h"
#include "QGraphCanvas.h"

#include "graphviz/cgraph.h"
#include "graphviz/types.h"

#include <QSet>

class QGraphNodePrivate
{
    Q_DECLARE_PUBLIC(QGraphNode)

    friend class QGraphEdgePrivate;

public:

    explicit QGraphNodePrivate(const QString& name, QGraphCanvasPrivate* canvas, QGraphNode* parent = 0);
    explicit QGraphNodePrivate(Agnode_t* node, QGraphNode* parent = 0);

private:

    Agnode_t* node() const;

    QRectF boundingRect() const;

    void updateState();

    enum Styles { Default, Filled, Invisible, Diagonals, Rounded, Dashed, Dotted, Solid, Bold };

    void setAttribute(const QString &name, const QString &value);
    QVariant attribute(const QString &name) const;
    QPointF point(const pointf& coord) const;
    QSet< Styles > style(const QString& str) const;
    double height() const;
    QList<QPolygonF> polygon(const polygon_t *poly, const bool close = false) const;
    QPainterPath path(Agnode_t* n) const;
    void drawLabel(QPainter* painter) const;

private:

    QGraphNode* const q_ptr;

    // node specfication
    Agnode_t* m_node;

    // node state after layout
    QSet< Styles > m_styles;
    QColor m_fillColor;
    QColor m_penColor;
    QPainterPath m_path;
    QRectF m_boundingBox;

};

#endif // QGRAPHNODEPRIVATE_H

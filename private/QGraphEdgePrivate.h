/*!
   \file QGraphEdgePrivate.h
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

#ifndef QGRAPHEDGEPRIVATE_H
#define QGRAPHEDGEPRIVATE_H

#include "QGraphEdge.h"
#include "QGraphNode.h"
#include "QGraphCanvas.h"

#include "graphviz/cgraph.h"
#include "graphviz/types.h"

class QGraphEdgePrivate
{
    Q_DECLARE_PUBLIC(QGraphEdge)

public:

    explicit QGraphEdgePrivate(const QString& name, QGraphCanvasPrivate* canvas, QGraphNodePrivate* head, QGraphNodePrivate* tail, QGraphEdge* parent = 0);
    explicit QGraphEdgePrivate(Agedge_t* edge, QGraphEdge* parent = 0);

protected:

    void setAttribute(const QString &name, const QString &value);

    void updateState();

private:

    QRectF boundingRect() const;
    double height() const;
    QPointF point(const pointf& coord) const;
    QPainterPath path() const;
    QPainterPath labelPath(QColor& fontcolor, double& fontsize, QFont& font) const;
    void drawLabel(QPainter* painter) const;

private:

    QGraphEdge* const q_ptr;

    // edge specification
    Agedge_t* m_edge;

    // logical DPI Y of painter device (determined in constructor)
    int m_logicalDpiY;

    // edge state after layout
    QPainterPath m_path;
    QPainterPath m_labelPath;
    QFont m_font;
    QColor m_fontColor;
    double m_fontSize;
    QRectF m_boundingBox;

};

#endif // QGRAPHEDGEPRIVATE_H

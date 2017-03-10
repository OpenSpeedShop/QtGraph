/*!
   \file QGraphCanvasPrivate.h
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

#ifndef QGRAPHCANVASPRIVATE_H
#define QGRAPHCANVASPRIVATE_H

#include "QGraphCanvas.h"

#include "graphviz/cgraph.h"
#include "graphviz/gvc.h"


class QGraphCanvasPrivate
{
    Q_DECLARE_PUBLIC(QGraphCanvas)

    friend class QGraphNodePrivate;
    friend class QGraphEdgePrivate;

public:

    explicit QGraphCanvasPrivate(const QString &name,
                                 const QGraphCanvas::NameValueList& graphAttributeSettings,
                                 const QGraphCanvas::NameValueList& nodeAttributeSettings,
                                 const QGraphCanvas::NameValueList& edgeAttributeSettings,
                                 QGraphCanvas* parent = 0);
    explicit QGraphCanvasPrivate(const char* data,
                                 const QGraphCanvas::NameValueList& graphAttributeSettings,
                                 const QGraphCanvas::NameValueList& nodeAttributeSettings,
                                 const QGraphCanvas::NameValueList& edgeAttributeSettings,
                                 QGraphCanvas* parent = 0);
private:

    QRectF boundingBox() const;

    Agraph_t* graph() const;

    void updateLayout();

    void setAttributes(const QGraphCanvas::NameValueList& graphAttributeSettings,
                       const QGraphCanvas::NameValueList& nodeAttributeSettings,
                       const QGraphCanvas::NameValueList& edgeAttributeSettings);

    void setGraphAttribute(const QString &name, const QString &value);
    void setNodeAttribute(const QString &name, const QString &value);
    void setEdgeAttribute(const QString &name, const QString &value);

    void showGrid(bool shown);
    bool drawGrid() const { return m_drawGrid; }

protected:

    QGraphCanvas* const q_ptr;

private:

    GVC_t* m_gvc;
    Agraph_t* m_graph;
    bool m_drawGrid;

};

#endif // QGRAPHCANVASPRIVATE_H

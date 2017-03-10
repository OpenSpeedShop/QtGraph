/*!
   \file QGraphCanvas.h
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

#ifndef QGRAPHCANVAS_H
#define QGRAPHCANVAS_H

#include <QGraphicsScene>
#include <QList>
#include <QPair>

#include "argonavis-lib-config.h"
#include "qtgraphlibrary.h"

class QGraphCanvasPrivate;
class QGraphNode;
class QGraphEdge;
class QGraphicsObject;


class QTGRAPH_EXPORT QGraphCanvas : public QGraphicsScene
{
    Q_OBJECT
    Q_DISABLE_COPY(QGraphCanvas)
    Q_DECLARE_PRIVATE(QGraphCanvas)

    friend class QGraphNode;
    friend class QGraphEdge;

public:

    typedef QPair< QString, QString > NameValuePair;
    typedef QList< NameValuePair > NameValueList;

    explicit QGraphCanvas(const QString& name,
                          const NameValueList& graphAttributeSettings = NameValueList(),
                          const NameValueList& nodeAttributeSettings = NameValueList(),
                          const NameValueList& edgeAttributeSettings = NameValueList(),
                          QObject *parent = Q_NULLPTR);
    explicit QGraphCanvas(const char* data,
                          const NameValueList& graphAttributeSettings = NameValueList(),
                          const NameValueList& nodeAttributeSettings = NameValueList(),
                          const NameValueList& edgeAttributeSettings = NameValueList(),
                          QObject *parent = Q_NULLPTR);
    explicit QGraphCanvas(const QString& name,
                          const QRectF& sceneRect,
                          const NameValueList& graphAttributeSettings = NameValueList(),
                          const NameValueList& nodeAttributeSettings = NameValueList(),
                          const NameValueList& edgeAttributeSettings = NameValueList(),
                          QObject *parent = Q_NULLPTR);
    explicit QGraphCanvas(const QString& name,
                          qreal x, qreal y, qreal width, qreal height,
                          const NameValueList& graphAttributeSettings = NameValueList(),
                          const NameValueList& nodeAttributeSettings = NameValueList(),
                          const NameValueList& edgeAttributeSettings = NameValueList(),
                          QObject *parent = Q_NULLPTR);
    ~QGraphCanvas();

    void addGraphNode(QGraphNode *node);
    void addGraphEdge(QGraphEdge *edge);

    void updateLayout();

    void showGrid(bool shown);

signals:

    void layoutUpdated();

    void graphNodeSelected(QGraphNode* node);
    void graphEdgeSelected(QGraphEdge* edge);

protected:

    void addItem(QGraphicsItem *item);
    void drawBackground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;

    void addGraphNode(void *node);
    void addGraphEdge(void *edge);

private slots:

    void handleLayoutUdated();
    void handleSelectionChanged();

protected:

    QGraphCanvasPrivate* const d_ptr;

};

#endif // QGRAPHCANVAS_H

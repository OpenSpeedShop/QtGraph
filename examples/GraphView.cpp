/*!
   \file GraphView.cpp
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

#include "GraphView.h"

#include <QWheelEvent>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtMath>
#else
#include "qmath.h"
#endif


GraphView::GraphView(QWidget *parent)
    : QGraphicsView( parent )
{

}

GraphView::GraphView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView( scene, parent )
{

}

void GraphView::wheelEvent(QWheelEvent* event)
{
    qreal scaleFactor = qPow( 2.0, event->delta() / 240.0 ); // how fast zooming occurs
    qreal factor = transform().scale( scaleFactor, scaleFactor ).mapRect( QRectF(0, 0, 1, 1) ).width();
    if ( 0.05 < factor && factor < 10.0 ) {
        // limit zoom extents
        scale( scaleFactor, scaleFactor );
    }
}

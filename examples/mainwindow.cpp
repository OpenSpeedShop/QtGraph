/*!
   \file MainWindow.cpp
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

#include "mainwindow.h"

#include "ui_mainwindow.h"

#include "QtGraph/QGraphCanvas.h"
#include "QtGraph/QGraphNode.h"
#include "QtGraph/QGraphEdge.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow( parent )
    , ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    const std::string digraphStr =
            "digraph G {"
            "0 [label=\"main\", shape=\"square\", file=\"mutatee.c\", line=\"43\", unit=\"mutatee\"];"
            "1 [label=\"work\", file=\"mutatee.c\", line=\"33\", unit=\"mutatee\"];"
            "2 [label=\"f3\", file=\"mutatee.c\", line=\"24\", unit=\"mutatee\"];"
            "3 [label=\"f2\", file=\"mutatee.c\", line=\"15\", unit=\"mutatee\"];"
            "4 [label=\"f1\", file=\"mutatee.c\", line=\"6\", unit=\"mutatee\"];"
            "0->1  [label=\"0\"];"
            "1->2  [label=\"50\"];"
            "1->3  [label=\"36.3636\"];"
            "1->4  [label=\"13.6364\"];"
            "}";

    // create graph
#ifdef MANUAL_GRAPH_CONSTRUCTION_EXAMPLE
    QGraphCanvas* g = new QGraphCanvas( QStringLiteral("calltree") );
    QGraphNode* n1 = new QGraphNode( QStringLiteral("main"), g );
    g->addGraphNode( n1 );

    QGraphNode* n2 = new QGraphNode( QStringLiteral("runTest"), g );
    g->addGraphNode( n2 );

    QGraphEdge* e1 = new QGraphEdge( QStringLiteral("1"), g, n1, n2 );
    g->addGraphEdge( e1 );

    QGraphNode* n3 = new QGraphNode( QStringLiteral("matrixMul_coalescing"), g );
    g->addGraphNode( n3 );
    QGraphEdge* e2 = new QGraphEdge( QStringLiteral("1"), g, n2, n3 );
    g->addGraphEdge( e2 );

    QGraphNode* n4 = new QGraphNode( QStringLiteral("matrixMul_naive"), g );
    g->addGraphNode( n4 );
    QGraphEdge* e3 = new QGraphEdge( QStringLiteral("1"), g, n2, n4 );
    g->addGraphEdge( e3 );

    QGraphNode* n5 = new QGraphNode( QStringLiteral("matrixMul_tiling"), g );
    g->addGraphNode( n5 );
    QGraphEdge* e4 = new QGraphEdge( QStringLiteral("1"), g, n2, n5 );
    g->addGraphEdge( e4 );

    QGraphNode* n6 = new QGraphNode( QStringLiteral("matrixMul_noBankConflict"), g );
    g->addGraphNode( n6 );
    QGraphEdge* e5 = new QGraphEdge( QStringLiteral("1"), g, n2, n6 );
    g->addGraphEdge( e5 );

    QGraphNode* n7 = new QGraphNode( QStringLiteral("matrixMul_compOpt"), g );
    g->addGraphNode( n7 );
    QGraphEdge* e6 = new QGraphEdge( QStringLiteral("1"), g, n2, n7 );
    g->addGraphEdge( e6 );

    QGraphNode* n8 = new QGraphNode( QStringLiteral("matrixMul_prefetch"), g );
    g->addGraphNode( n8 );
    QGraphEdge* e7 = new QGraphEdge( QStringLiteral("1"), g, n2, n8 );
    g->addGraphEdge( e7 );

    QGraphNode* n9 = new QGraphNode( QStringLiteral("matrixMul_unroll"), g );
    g->addGraphNode( n9 );
    QGraphEdge* e8 = new QGraphEdge( QStringLiteral("1"), g, n2, n9 );
    g->addGraphEdge( e8 );
#else
    QGraphCanvas* g = new QGraphCanvas( digraphStr.data() );
#endif

    // uncomment line below to show graph paper-like grid background within graph canvas rectangle
    //g->showGrid( true );

    g->updateLayout();

    ui->graphicsView->setScene( g );
}

MainWindow::~MainWindow()
{
    delete ui;
}

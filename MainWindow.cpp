#include "MainWindow.h"

#include "ui_MainWindow.h"

#include <QGraphCanvas.h>
#include <QGraphNode.h>
#include <QGraphEdge.h>

#include "common/argonavis-lib-config.h"

#include <QDebug>

#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>

const std::string digraphStr =
        "digraph G {"
        "0 [label=\"address:0x00400469\nvalue:0\n__start\"];"
        "1 [label=\"address:0x00007f1499874d7b\nvalue:0\nlibmonitor __libc_start_main\"];"
        "2 [label=\"address:0x00007f14992bad65\nvalue:0\nlibc __libc_start_main\"];"
        "3 [label=\"address:0x00007f14998749bc\nvalue:0\nmonitor_main\"];"
        "4 [label=\"address:0x0040066a\nvalue:0\nmain /home/dpm/TEST/mutatee.c:46\"];"
        "5 [label=\"address:0x00400606\nvalue:255\nwork /home/dpm/TEST/mutatee.c:38\"];"
        "6 [label=\"address:0x004005cb\nvalue:255\nf3 /home/dpm/TEST/mutatee.c:27\"];"
        "7 [label=\"address:0x00400617\nvalue:255\nwork /home/dpm/TEST/mutatee.c:38\"];"
        "8 [label=\"address:0x00400595\nvalue:255\nf2 /home/dpm/TEST/mutatee.c:18\"];"
        "9 [label=\"address:0x00400628\nvalue:255\nwork /home/dpm/TEST/mutatee.c:38\"];"
        "10 [label=\"address:0x0040055f\nvalue:255\nf1 /home/dpm/TEST/mutatee.c:9\"];"
        "11 [label=\"address:0x00400587\nvalue:1\nf2 /home/dpm/TEST/mutatee.c:19\"];"
        "12 [label=\"address:0x0040058e\nvalue:15\nf2 /home/dpm/TEST/mutatee.c:18\"];"
        "13 [label=\"address:0x004005c4\nvalue:16\nf3 /home/dpm/TEST/mutatee.c:27\"];"
        "14 [label=\"address:0x00400558\nvalue:3\nf1 /home/dpm/TEST/mutatee.c:9\"];"
        "15 [label=\"address:0x0040054a\nvalue:37\nf1 /home/dpm/TEST/mutatee.c:10\"];"
        "16 [label=\"address:0x00400554\nvalue:42\nf1 /home/dpm/TEST/mutatee.c:9\"];"
        "17 [label=\"address:0x004005c0\nvalue:155\nf3 /home/dpm/TEST/mutatee.c:27\"];"
        "18 [label=\"address:0x004005b6\nvalue:156\nf3 /home/dpm/TEST/mutatee.c:28\"];"
        "19 [label=\"address:0x0040058a\nvalue:99\nf2 /home/dpm/TEST/mutatee.c:18\"];"
        "20 [label=\"address:0x00400580\nvalue:86\nf2 /home/dpm/TEST/mutatee.c:19\"];"
        "0->1  [label=0];"
        "1->2  [label=0];"
        "2->3  [label=0];"
        "3->4  [label=0];"
        "4->5  [label=0];"
        "4->7  [label=0];"
        "4->9  [label=0];"
        "5->6  [label=771];"
        "5->13  [label=16];"
        "5->17  [label=155];"
        "5->18  [label=156];"
        "7->8  [label=544];"
        "7->11  [label=1];"
        "7->12  [label=15];"
        "7->19  [label=99];"
        "7->20  [label=86];"
        "9->10  [label=285];"
        "9->14  [label=3];"
        "9->15  [label=37];"
        "9->16  [label=42];"
        "}";

const std::string digraphStr2 =
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

const std::string digraphStr3 =
        "digraph G {"
        "rankdir=LR;"
        "node [shape=box, color=blue];"
        "node1 [style=filled];"
        "node2 [style=filled, fillcolor=red];"
        "node3 [style=invisible];"
        "node0 -> node1 -> node2;"
        "}";


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow( parent )
    , ui( new Ui::MainWindow )
{
    ui->setupUi( this );

#if 0
    // configure graph attributes
    setNodeAttribute( QStringLiteral("style"), QStringLiteral("filled") );
    setNodeAttribute( QStringLiteral("fillcolor"), QStringLiteral("green") );
    setGraphAttribute( QStringLiteral("splines"), QStringLiteral("ortho") );
    setGraphAttribute( QStringLiteral("rankdir"), QStringLiteral("LR") );
    setGraphAttribute( QStringLiteral("nodesep"), QStringLiteral("0.4") );
    setGraphAttribute( QStringLiteral("pad"), QStringLiteral("0.1") );

    setNodeAttribute( QStringLiteral("shape"), QStringLiteral("box") );
    setNodeAttribute( QStringLiteral("style"), QStringLiteral("filled") );
    setNodeAttribute( QStringLiteral("fillcolor"), QStringLiteral("white") );
    setNodeAttribute( QStringLiteral("height"), QStringLiteral("1.2") );

    setEdgeAttribute( QStringLiteral("minlen"), QStringLiteral("3") );
#endif

    // set graph attributes
    QGraphCanvas::NameValueList graphAttributeList;
    //graphAttributeList.push_back( qMakePair( QStringLiteral("rankdir"), QStringLiteral("LR") ) );
    graphAttributeList.push_back( qMakePair( QStringLiteral("nodesep"), QStringLiteral("0.4") ) );
    //graphAttributeList.push_back( qMakePair( QStringLiteral("splines"), QStringLiteral("ortho") ) );

    // set default node attributes
    QGraphCanvas::NameValueList nodeAttributeList;
    //nodeAttributeList.push_back( qMakePair( QStringLiteral("shape"), QStringLiteral("box") ) );
    nodeAttributeList.push_back( qMakePair( QStringLiteral("style"), QStringLiteral("filled") ) );
    nodeAttributeList.push_back( qMakePair( QStringLiteral("fillcolor"), QStringLiteral("green") ) );

    // set default edge attributes
    QGraphCanvas::NameValueList edgeAttributeList;
    //nodeAttributeList.push_back( qMakePair( QStringLiteral("minlen"), QStringLiteral("30") ) );

    // create graph
#if 0
    QGraphCanvas* g = new QGraphCanvas( QStringLiteral("calltree"), graphAttributeList, nodeAttributeList, edgeAttributeList );
    QGraphNode* n1 = new QGraphNode( QStringLiteral("main (matrixmul)"), g );
    g->addGraphNode( n1 );

    QGraphNode* n2 = new QGraphNode( QStringLiteral("runTest(int, char**) (matrixmul)"), g );
    g->addGraphNode( n2 );

    QGraphEdge* e1 = new QGraphEdge( QStringLiteral("edge1"), g, n1, n2 );
    g->addGraphEdge( e1 );

    QGraphNode* n3 = new QGraphNode( QStringLiteral("matrixMul_coalescing(float*, float*, float*, int, int) (matrixmul)"), g );
    g->addGraphNode( n3 );
    QGraphEdge* e2 = new QGraphEdge( QStringLiteral("edge2"), g, n2, n3 );
    g->addGraphEdge( e2 );

    QGraphNode* n4 = new QGraphNode( QStringLiteral("matrixMul_naive(float*, float*, float*, int, int) (matrixmul)"), g );
    g->addGraphNode( n4 );
    QGraphEdge* e3 = new QGraphEdge( QStringLiteral("edge3"), g, n2, n4 );
    g->addGraphEdge( e3 );

    QGraphNode* n5 = new QGraphNode( QStringLiteral("matrixMul_tiling(float*, float*, float*, int, int) (matrixmul)"), g );
    g->addGraphNode( n5 );
    QGraphEdge* e4 = new QGraphEdge( QStringLiteral("edge4"), g, n2, n5 );
    g->addGraphEdge( e4 );

    QGraphNode* n6 = new QGraphNode( QStringLiteral("matrixMul_noBankConflict(float*, float*, float*, int, int) (matrixmul)"), g );
    g->addGraphNode( n6 );
    QGraphEdge* e5 = new QGraphEdge( QStringLiteral("edge5"), g, n2, n6 );
    g->addGraphEdge( e5 );

    QGraphNode* n7 = new QGraphNode( QStringLiteral("matrixMul_compOpt(float*, float*, float*, int, int) (matrixmul)"), g );
    g->addGraphNode( n7 );
    QGraphEdge* e6 = new QGraphEdge( QStringLiteral("edge6"), g, n2, n7 );
    g->addGraphEdge( e6 );

    QGraphNode* n8 = new QGraphNode( QStringLiteral("matrixMul_prefetch(float*, float*, float*, int, int) (matrixmul)"), g );
    g->addGraphNode( n8 );
    QGraphEdge* e7 = new QGraphEdge( QStringLiteral("edge7"), g, n2, n8 );
    g->addGraphEdge( e7 );

    QGraphNode* n9 = new QGraphNode( QStringLiteral("matrixMul_unroll(float*, float*, float*, int, int) (matrixmul)"), g );
    g->addGraphNode( n9 );
    QGraphEdge* e8 = new QGraphEdge( QStringLiteral("edge8"), g, n2, n9 );
    g->addGraphEdge( e8 );
#else
    QGraphCanvas* g = new QGraphCanvas( digraphStr.data(), graphAttributeList, nodeAttributeList, edgeAttributeList );
#endif
    g->updateLayout();

    ui->graphicsView->setScene( g );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawGraph()
{
    QRectF sceneRect = ui->graphicsView->sceneRect();

    // Fit in view
    ui->graphicsView->fitInView( sceneRect, Qt::KeepAspectRatio );
}

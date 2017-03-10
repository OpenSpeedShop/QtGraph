#-------------------------------------------------
#
# Project created by QtCreator 2017-02-22T20:49:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtGraph-example
TEMPLATE = app

# Uncomment below if manual graph construction example is desired
#DEFINES += MANUAL_GRAPH_CONSTRUCTION_EXAMPLE

INSTALL_ROOT = $$(INSTALL_ROOT)
GRAPHVIZ_ROOT = $$(GRAPHVIZ_ROOT)

INCLUDEPATH += $$GRAPHVIZ_ROOT/include/graphviz
LIBS += -L$$GRAPHVIZ_ROOT/lib -lcdt -lgvc -lcgraph

INCLUDEPATH += $$INSTALL_ROOT/include $$INSTALL_ROOT/include/QtGraph
CONFIG(debug, debug|release) {
LIBS += -L$$INSTALL_ROOT/lib64/$$QT_VERSION -lQtGraphd
}
CONFIG(release, debug|release) {
LIBS += $$INSTALL_ROOT/lib64/$$QT_VERSION -lQtGraph
}

SOURCES += main.cpp \
        mainwindow.cpp \
    GraphView.cpp

HEADERS  += mainwindow.h \
    GraphView.h

FORMS    += mainwindow.ui

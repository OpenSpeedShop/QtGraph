#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void drawGraph();

private slots:

private:

    Ui::MainWindow *ui;
    QGraphicsScene *_scene;

};

#endif // MAINWINDOW_H

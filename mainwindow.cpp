#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent),
     fsExplWdgt { new FSExploreWidget(this) }
{

}

MainWindow::~MainWindow()
{
    delete fsExplWdgt;
}


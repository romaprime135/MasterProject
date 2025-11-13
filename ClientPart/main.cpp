#include "mainwindow.h"

#include <QApplication>
#include <QStatusBar>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedSize(800, 600);
    w.statusBar()->setSizeGripEnabled(false);
    w.show();
    return a.exec();
}

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    // Query the statuses after the GUI is loaded
    w.getSlaveStatus();

    return a.exec();
}

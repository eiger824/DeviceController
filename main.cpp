#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    // Query the statuses after the GUI is loaded
    w.getSlaveStatus();
    w.printSlaves();

    return a.exec();
}

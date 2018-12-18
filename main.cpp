#include "mainwindow.h"
#include <QApplication>

//sudo chown sub1 /dev/ttyACM0

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

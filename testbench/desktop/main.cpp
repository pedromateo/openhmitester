#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication qapp(argc, argv);
    MainWindow w;
    w.show();

    return qapp.exec();
}

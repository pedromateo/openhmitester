#include <QApplication>
#include "mainwindow.h"

#include <qtpreloadingcontrol.h>


int main(int argc, char *argv[])
{
    QApplication qapp(argc, argv);

    /// manual preloading
    QtPreloadingControl::do_preload();
    ///

    MainWindow w;
    w.show();

    return qapp.exec();
}

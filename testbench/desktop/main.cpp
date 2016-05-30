#include <QApplication>
#include "mainwindow.h"

#include <qtx11preloadingcontrol.h>

int main(int argc, char *argv[])
{
    QApplication qapp(argc, argv);

    /// manual preloading

    QtX11PreloadingControl::do_preload();

    ///

    MainWindow w;
    w.show();

    return qapp.exec();
}

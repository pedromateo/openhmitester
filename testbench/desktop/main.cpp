#include <QApplication>
#include "mainwindow.h"

#if MANUAL_PRELOAD
#include <qtpreloadingcontrol.h>
#endif

int main(int argc, char *argv[])
{
    QApplication qapp(argc, argv);

#if MANUAL_PRELOAD
    /// manual preloading
    QtPreloadingControl::Do_preload();
#endif

    MainWindow w;
    w.show();

    return qapp.exec();
}

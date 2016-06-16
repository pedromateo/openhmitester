#-------------------------------------------------
# Project created by QtCreator 2010-03-10T08:55:24
#-------------------------------------------------

equals(QT_MAJOR_VERSION, 5) {
  message(" Simusaes in QT 5 ")
   QT += widgets
   CONFIG +=
}
else {
  message(" Simusaes in QT 4 ")
   QT +=
   CONFIG +=
}

TARGET = simusaes
TEMPLATE = app


SOURCES += main.cpp \
           mainwindow.cpp \
           nativecontroller.cpp

HEADERS += mainwindow.h \
            nativecontroller.h

FORMS   += mainwindow.ui

#LIBS += -lboost_system -lboost_program_options


######################################################################
### Includes needed to manually include qt_win_lib_preload into AUT
######################################################################

#CONFIG += MANUAL_PRELOAD

MANUAL_PRELOAD{

    message(Simulating manual preload. Including additional libs and sources.)

    QT +=  testlib


    INCLUDEPATH += ../../../openhmitester/src/win/qt_win_lib_preload/
    INCLUDEPATH += ../../../openhmitester/src/linux/qt_win_lib_preload/
    INCLUDEPATH += ../../../openhmitester/src/preloaders
    INCLUDEPATH += ../../../openhmitester/src/common
    INCLUDEPATH += ../../../openhmitester/src/lib_preload
    INCLUDEPATH += ../../../openhmitester/src/qt_lib_preload


    include(../../../openhmitester/src/qt_lib_preload/qt_lib_preload.pri)

    SOURCES += ../../../openhmitester/src/preloaders/qtpreloadingcontrol.cpp
    HEADERS += ../../../openhmitester/src/preloaders/qtpreloadingcontrol.h


    linux{
        LIBS += -L../../src/linux/qt_linux_lib_preload
        LIBS += -lqt_linux_oht_preload

        LIBS += -L/opt/boost/boost_1_60_0/lib/
        INCLUDEPATH += /opt/boost/boost_1_60_0/include/
        LIBS += -lboost_serialization

    }

    win32{
        LIBS += -LC:/boost/lib
        INCLUDEPATH += C:/boost/include/
        LIBS += -lboost_serialization-mgw49-mt-d-1_60

        CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../src/win/qt_win_lib_preload/release/ -lqt_win_oht_preload
        else:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../src/win/qt_win_lib_preload/debug/ -lqt_win_oht_preload

    }

}

####################################################################






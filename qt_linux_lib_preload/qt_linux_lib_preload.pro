# -------------------------------------------------
# Project created by QtCreator 2009-01-22T09:35:51
# -------------------------------------------------

#
# HMITester and OHTLibPreload common sources
#


equals(QT_MAJOR_VERSION, 4) {

    include(../common/common.pri)
}

equals(QT_MAJOR_VERSION, 5) {

    message(" === FIXME === This bug have to be fixed for Qt5")

    INCLUDEPATH += ../common/

    SOURCES += ../common/datamodel.cpp \
               ../common/comm.cpp \
               ../common/messageclientserver.cpp \
               ../common/utilclasses.cpp \
               ../common/uuid.cpp \
               ../common/controlsignaling.cpp

    HEADERS += ../common/datamodel.h \
               ../common/comm.h \
               ../common/messageclientserver.h \
               ../common/utilclasses.h \
               ../common/uuid.h \
               ../common/controlsignaling.h \
               ../common/ohtbaseconfig.h \
               ../common/debug.h
}

####
#### base Lib Preload project
####


equals(QT_MAJOR_VERSION, 4) {

    include(../lib_preload/lib_preload.pri)
}

equals(QT_MAJOR_VERSION, 5) {

    message(" === FIXME === This bug have to be fixed for Qt5")

    INCLUDEPATH += ../lib_preload/

    SOURCES += ../lib_preload/preloadcontroller.cpp \
        ../lib_preload/eventconsumer.cpp \
        ../lib_preload/preloadingcontrol.cpp

    HEADERS += ../lib_preload/preloadcontroller.h \
        ../lib_preload/LibPreload_global.h \
        ../lib_preload/eventconsumer.h \
        ../lib_preload/eventexecutor.h \
        ../lib_preload/preloadingcontrol.h

    LIBS += -lboost_serialization
}


####
#### qt-linux Lib Preload project
####


equals(QT_MAJOR_VERSION, 5) {
  message(" ================ QT 5 ================ ")
   QT += widgets testlib
   CONFIG +=
}
else {
  message(" ================ QT 4 ================ ")
   QT +=
   CONFIG += qtestlib
}

QT += network xml
CONFIG += debug

TARGET = qt_linux_oht_preload
TEMPLATE = lib


###
### qt-linux Lib Preload adaptation sources
###

SOURCES += qteventconsumer.cpp \
    qteventexecutor.cpp \
    qtx11preloadingcontrol.cpp \
    qtownevents.cpp \
    qwidgetutils.cpp \
    qwidgetadapter.cpp
HEADERS += qteventconsumer.h \
    qteventexecutor.h \
    qtx11preloadingcontrol.h \
    qtownevents.h \
    qwidgetutils.h \
    qwidgetadapter.h


###
### qt-linux Lib Preload libs
###

LIBS += -lX11


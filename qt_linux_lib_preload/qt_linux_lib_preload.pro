# -------------------------------------------------
# Project created by QtCreator 2009-01-22T09:35:51
# -------------------------------------------------

#
# HMITester and OHTLibPreload common sources
#

include(../common/common.pri)

####
#### base Lib Preload project
####

include(../lib_preload/lib_preload.pri)


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
    qtownevents.cpp \
    qwidgetutils.cpp \
    qwidgetadapter.cpp
HEADERS += qteventconsumer.h \
    qteventexecutor.h \
    qtownevents.h \
    qwidgetutils.h \
    qwidgetadapter.h

###
### linux preloading
###

SOURCES += ../preloaders/qtx11preloadingcontrol.cpp
HEADERS += ../preloaders/qtx11preloadingcontrol.h

###
### qt-linux Lib Preload libs
###

LIBS += -lX11

LIBS += -L/opt/boost/boost_1_60_0/lib/
INCLUDEPATH += /opt/boost/boost_1_60_0/include/


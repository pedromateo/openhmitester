# -------------------------------------------------
# Project created by QtCreator 2009-01-22T09:35:51
# -------------------------------------------------


####
#### base Lib Preload project
####

INCLUDEPATH += ../lib_preload/
DEPENDPATH += ../lib_preload/

include(../lib_preload/lib_preload.pro)

####
#### qt-linux Lib Preload project
####


QT += network xml

CONFIG += qtestlib debug

TARGET = OHTPreload
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


# -------------------------------------------------
# Project created by QtCreator 2009-01-22T09:35:51
# -------------------------------------------------

QT += network xml
CONFIG += qtestlib debug

TARGET = OHTPreload
TEMPLATE = lib

DEFINES += LIBPRELOAD_LIBRARY

###
###
###

SOURCES += preloadcontroller.cpp \
    eventconsumer.cpp \
    preloadingcontrol.cpp

HEADERS += preloadcontroller.h \
    LibPreload_global.h \
    eventconsumer.h \
    eventexecutor.h \
    preloadingcontrol.h

LIBS += -lboost_serialization

#
# HMITester and OHTLibPreload common sources
#

INCLUDEPATH += ../common
DEPENDPATH += ../common

SOURCES += datamodel.cpp \
           comm.cpp \
           utilclasses.cpp \
           uuid.cpp \
           controlsignaling.cpp

HEADERS += datamodel.h \
           comm.h \
           utilclasses.h \
           uuid.h \
           controlsignaling.h \
           ohtbaseconfig.h \
           debug.h

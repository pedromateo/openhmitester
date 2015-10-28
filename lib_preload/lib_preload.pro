# -------------------------------------------------
# Project created by QtCreator 2009-01-22T09:35:51
# -------------------------------------------------


equals(QT_MAJOR_VERSION, 5) {
  message(" ================ QT 5 ================ ")
   QT += testlib
   CONFIG +=
}
else {
  message(" ================ QT 4 ================ ")
   QT +=
   CONFIG += qtestlib
}


QT += network xml
CONFIG += debug

TARGET = OHTPreload
TEMPLATE = lib

DEFINES += LIBPRELOAD_LIBRARY

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

###
### OHT LibPreload sources
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

OTHER_FILES += \
    lib_preload.pri

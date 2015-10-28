# -------------------------------------------------
# Project created by QtCreator 2008-12-12T12:28:02
# -------------------------------------------------


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

QT += script network
#CONFIG += debug

TARGET = HMITester
TEMPLATE = lib



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
### HMITester  sources
###

SOURCES += hmitestercontrol.cpp \
    playbackcontrol.cpp \
    recordingcontrol.cpp \
    processcontrol.cpp \
    datamodelmanager.cpp \
    executionthread.cpp \
    itemmanager.cpp \
    generalinputdialog.cpp

HEADERS += hmitestercontrol.h \
    executionobserver.h \
    playbackcontrol.h \
    recordingcontrol.h \
    preloadingaction.h \
    processcontrol.h \
    datamodelmanager.h \
    datamodeladapter.h \
    executionthread.h \
    itemmanager.h \
    generalinputdialog.h \
    executionobserver.h \
    recordingobserver.h \
    exceptions.h

SOURCES += qtutils.cpp
HEADERS += qtutils.h

FORMS += hmitestercontrol.ui \
    generalinputdialog.ui

LIBS += -lboost_thread -lboost_system -lboost_serialization

OTHER_FILES += LICENSE.txt \
    hmi_tester.pri

RESOURCES += resources.qrc

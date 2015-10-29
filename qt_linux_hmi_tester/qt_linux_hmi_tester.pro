# -------------------------------------------------
# Project created by QtCreator 2008-12-12T12:28:02
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
#### base HMI Tester project
####


equals(QT_MAJOR_VERSION, 4) {

    include(../hmi_tester/hmi_tester.pri)
}

equals(QT_MAJOR_VERSION, 5) {

    message(" === FIXME === This bug have to be fixed for Qt5")

    INCLUDEPATH += ../hmi_tester/

    SOURCES += ../hmi_tester/hmitestercontrol.cpp \
        ../hmi_tester/playbackcontrol.cpp \
        ../hmi_tester/recordingcontrol.cpp \
        ../hmi_tester/processcontrol.cpp \
        ../hmi_tester/datamodelmanager.cpp \
        ../hmi_tester/executionthread.cpp \
        ../hmi_tester/itemmanager.cpp \
        ../hmi_tester/newtsdialog.cpp \
        ../hmi_tester/newtcdialog.cpp

    HEADERS += ../hmi_tester/hmitestercontrol.h \
        ../hmi_tester/executionobserver.h \
        ../hmi_tester/playbackcontrol.h \
        ../hmi_tester/recordingcontrol.h \
        ../hmi_tester/preloadingaction.h \
        ../hmi_tester/processcontrol.h \
        ../hmi_tester/datamodelmanager.h \
        ../hmi_tester/datamodeladapter.h \
        ../hmi_tester/executionthread.h \
        ../hmi_tester/itemmanager.h \
        ../hmi_tester/newtsdialog.h \
        ../hmi_tester/newtcdialog.h \
        ../hmi_tester/executionobserver.h \
        ../hmi_tester/recordingobserver.h \
        ../hmi_tester/exceptions.h

    SOURCES += ../hmi_tester/qtutils.cpp
    HEADERS += ../hmi_tester/qtutils.h

    FORMS += ../hmi_tester/hmitestercontrol.ui \
        ../hmi_tester/newtsdialog.ui \
        ../hmi_tester/newtcdialog.ui

    LIBS += -lboost_thread -lboost_system -lboost_serialization

    RESOURCES += ../hmi_tester/resources.qrc
}


####
#### qt-linux HMI Tester project
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

QT += script xml network
CONFIG += debug

TARGET = qt_linux_hmi_tester
TEMPLATE = app


###
### qt-linux adaptation sources
###

SOURCES += main.cpp \
           linuxpreloadingaction.cpp

HEADERS += linuxpreloadingaction.h \
           qtlinux_ohtconfig.h


###
### datamodel sources
###

SOURCES += xmldatamodeladapter.cpp
HEADERS += xmldatamodeladapter.h



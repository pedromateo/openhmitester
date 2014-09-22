# -------------------------------------------------
# Project created by QtCreator 2008-12-12T12:28:02
# -------------------------------------------------
QT += script \
    xml \
    network
CONFIG += qtestlib \
    debug
TARGET = HMITester
TEMPLATE = app
SOURCES += main.cpp \
    hmitestercontrol.cpp \
    playbackcontrol.cpp \
    recordingcontrol.cpp \
    comm.cpp \
    processcontrol.cpp \
    datamodelmanager.cpp \
    executionthread.cpp \
    itemmanager.cpp \
    utilclasses.cpp \
    linuxpreloadingaction.cpp \
    datamodel.cpp \
    xmldatamodeladapter.cpp \
    qtutils.cpp \
    controlsignaling.cpp \
    uuid.cpp \
    generalinputdialog.cpp
HEADERS += hmitestercontrol.h \
    executionobserver.h \
    playbackcontrol.h \
    recordingcontrol.h \
    preloadingaction.h \
    comm.h \
    processcontrol.h \
    datamodelmanager.h \
    datamodeladapter.h \
    executionthread.h \
    itemmanager.h \
    utilclasses.h \
    linuxpreloadingaction.h \
    datamodel.h \
    xmldatamodeladapter.h \
    qtutils.h \
    controlsignaling.h \
    uuid.h \
    debug.h \
    generalinputdialog.h \
    ohtconfig.h \
    executionobserver.h \
    recordingobserver.h
FORMS += hmitestercontrol.ui \
    generalinputdialog.ui
LIBS += -lboost_thread \
    -lboost_system \
    -lboost_serialization
OTHER_FILES += LICENSE.txt

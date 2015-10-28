
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

###
###
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

RESOURCES += resources.qrc

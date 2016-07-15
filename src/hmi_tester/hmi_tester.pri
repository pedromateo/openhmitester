
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

# directories ($$PWD) are set before files because of an error in Qt5

SOURCES += $$PWD/hmitestercontrol.cpp \
    $$PWD/playbackcontrol.cpp \
    $$PWD/recordingcontrol.cpp \
    $$PWD/processcontrol.cpp \
    $$PWD/datamodelmanager.cpp \
    $$PWD/executionthread.cpp \
    $$PWD/itemmanager.cpp \
    $$PWD/newtsdialog.cpp \
    $$PWD/edittsdialog.cpp \
    $$PWD/newtcdialog.cpp

HEADERS += $$PWD/hmitestercontrol.h \
    $$PWD/executionobserver.h \
    $$PWD/playbackcontrol.h \
    $$PWD/recordingcontrol.h \
    $$PWD/preloadingaction.h \
    $$PWD/processcontrol.h \
    $$PWD/datamodelmanager.h \
    $$PWD/datamodeladapter.h \
    $$PWD/executionthread.h \
    $$PWD/itemmanager.h \
    $$PWD/newtsdialog.h \
    $$PWD/edittsdialog.h \
    $$PWD/newtcdialog.h \
    $$PWD/executionobserver.h \
    $$PWD/recordingobserver.h \
    $$PWD/exceptions.h

SOURCES += $$PWD/qtutils.cpp
HEADERS += $$PWD/qtutils.h

FORMS += $$PWD/hmitestercontrol.ui \
    $$PWD/newtsdialog.ui \
    $$PWD/newtcdialog.ui \
    $$PWD/edittsdialog.ui

RESOURCES += $$PWD/resources.qrc

###
### Boost libs
###

# add here your boost includes if needed
linux {
    LIBS += -lboost_thread -lboost_system
}
win32 {
    LIBS += -lboost_thread-mgw49-mt-d-1_60 -lboost_system-mgw49-mt-d-1_60
}

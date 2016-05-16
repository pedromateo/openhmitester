
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
    $$PWD/newtcdialog.h \
    $$PWD/executionobserver.h \
    $$PWD/recordingobserver.h \
    $$PWD/exceptions.h

SOURCES += $$PWD/qtutils.cpp
HEADERS += $$PWD/qtutils.h

FORMS += $$PWD/hmitestercontrol.ui \
    $$PWD/newtsdialog.ui \
    $$PWD/newtcdialog.ui

RESOURCES += $$PWD/resources.qrc

###
### Boost libs
###

LIBS += -lboost_thread -lboost_system -lboost_serialization

# add here your boost includes if needed
LIBS += -L/opt/boost/boost_1_60_0/lib/
INCLUDEPATH += /opt/boost/boost_1_60_0/include/

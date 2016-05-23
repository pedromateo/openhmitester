
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD


# HMITester and OHTLibPreload common sources


include(../common/common.pri)


# directories ($$PWD) are set before files because of an error in Qt5

SOURCES += $$PWD/preloadcontroller.cpp \
    $$PWD/eventconsumer.cpp \
    $$PWD/preloadingcontrol.cpp

HEADERS += $$PWD/preloadcontroller.h \
    $$PWD/LibPreload_global.h \
    $$PWD/eventconsumer.h \
    $$PWD/eventexecutor.h \
    $$PWD/preloadingcontrol.h

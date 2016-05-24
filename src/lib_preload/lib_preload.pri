
# HMITester and OHTLibPreload common sources

include(../common/common.pri)


INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

# directories ($$PWD) are set before files because of an error in Qt5

SOURCES += $$PWD/preloadcontroller.cpp \
    $$PWD/eventconsumer.cpp \
    $$PWD/preloadingcontrol.cpp

HEADERS += $$PWD/preloadcontroller.h \
    $$PWD/LibPreload_global.h \
    $$PWD/eventconsumer.h \
    $$PWD/eventexecutor.h \
    $$PWD/preloadingcontrol.h

DEFINES += LIBPRELOAD_LIBRARY

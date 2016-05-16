
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

# directories ($$PWD) are set before files because of an error in Qt5


####
#### base Lib Preload project
####

include(../lib_preload/lib_preload.pri)


###
### qt-linux Lib Preload adaptation sources
###

SOURCES += $$PWD/qteventconsumer.cpp \
    $$PWD/qteventexecutor.cpp \
    $$PWD/qtownevents.cpp \
    $$PWD/qwidgetutils.cpp \
    $$PWD/qwidgetadapter.cpp
HEADERS += $$PWD/qteventconsumer.h \
    $$PWD/qteventexecutor.h \
    $$PWD/qtownevents.h \
    $$PWD/qwidgetutils.h \
    $$PWD/qwidgetadapter.h


###
### qt-linux Lib Preload libs
###

LIBS += -lX11

LIBS += -L/opt/boost/boost_1_60_0/lib/
INCLUDEPATH += /opt/boost/boost_1_60_0/include/

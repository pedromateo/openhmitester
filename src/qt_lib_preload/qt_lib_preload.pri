

# directories ($$PWD) are set before files because of an error in Qt5
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

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


linux {
    LIBS += -lX11
}
win32 {
    LIBS +=
}

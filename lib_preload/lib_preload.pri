
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

###
###
###

SOURCES += preloadcontroller.cpp \
    eventconsumer.cpp \
    preloadingcontrol.cpp

HEADERS += preloadcontroller.h \
    LibPreload_global.h \
    eventconsumer.h \
    eventexecutor.h \
    preloadingcontrol.h

LIBS += -lboost_serialization

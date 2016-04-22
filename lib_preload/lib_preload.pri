
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

###
### Boost libs
###

LIBS += -lboost_serialization

# add here your boost includes if needed
LIBS += -L/opt/boost/boost_1_60_0/lib/
INCLUDEPATH += /opt/boost/boost_1_60_0/include/

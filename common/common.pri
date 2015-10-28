
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

LIBS += -lboost_thread -lboost_system -lboost_serialization


SOURCES += datamodel.cpp \
           comm.cpp \
           messageclientserver.cpp \
           utilclasses.cpp \
           uuid.cpp \
           controlsignaling.cpp

HEADERS += datamodel.h \
           comm.h \
           messageclientserver.h \
           utilclasses.h \
           uuid.h \
           controlsignaling.h \
           ohtbaseconfig.h \
           debug.h

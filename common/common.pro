

equals(QT_MAJOR_VERSION, 5) {
  message(" ================ QT 5 ================ ")
   QT += testlib
   CONFIG +=
}
else {
  message(" ================ QT 4 ================ ")
   QT +=
   CONFIG += qtestlib
}

QT += script network
#CONFIG += debug

TARGET = OHTCommon
TEMPLATE = lib


LIBS += -lboost_thread -lboost_system -lboost_serialization


SOURCES += datamodel.cpp \
           comm.cpp \
           utilclasses.cpp \
           uuid.cpp \
           controlsignaling.cpp

HEADERS += datamodel.h \
           comm.h \
           utilclasses.h \
           uuid.h \
           controlsignaling.h \
           ohtbaseconfig.h \
           debug.h

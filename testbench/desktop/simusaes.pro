#-------------------------------------------------
# Project created by QtCreator 2010-03-10T08:55:24
#-------------------------------------------------

equals(QT_MAJOR_VERSION, 5) {
  message(" ================ QT 5 ================ ")
   QT += widgets
   CONFIG +=
}
else {
  message(" ================ QT 4 ================ ")
   QT +=
   CONFIG +=
}

TARGET = simusaes
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    nativecontroller.cpp

HEADERS  += mainwindow.h \
    nativecontroller.h

FORMS    += mainwindow.ui

#LIBS += -lboost_system -lboost_program_options


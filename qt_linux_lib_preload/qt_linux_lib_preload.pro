# -------------------------------------------------
# Project created by QtCreator 2009-01-22T09:35:51
# -------------------------------------------------

equals(QT_MAJOR_VERSION, 5) {
  message(" ================ QT 5 ================ ")
   QT += widgets testlib
   CONFIG +=
}else {
  message(" ================ QT 4 ================ ")
   QT +=
   CONFIG += qtestlib
}

QT += network xml
CONFIG += debug

TARGET = qt_linux_oht_preload
TEMPLATE = lib


####
#### Qt Lib Preload project
####

include(../qt_lib_preload/qt_lib_preload.pri)


###
### linux preloading sources
###

SOURCES += ../preloaders/qtx11preloadingcontrol.cpp
HEADERS += ../preloaders/qtx11preloadingcontrol.h




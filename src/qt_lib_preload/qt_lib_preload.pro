# -------------------------------------------------
# Project created by QtCreator 2009-01-22T09:35:51
# -------------------------------------------------


####
#### qt-linux Lib Preload project

equals(QT_MAJOR_VERSION, 5) {
  message(" ================ QT 5 ================ ")
   QT += widgets testlib network
   CONFIG +=
}
else {
  message(" ================ QT 4 ================ ")
   QT += network
   CONFIG += qtestlib
}

CONFIG += debug

TARGET = qt_oht_preload
TEMPLATE = lib

####
#### base Lib Preload project

include(../lib_preload/lib_preload.pri)


###
### OHT Qt LibPreload sources

include(qt_lib_preload.pri)


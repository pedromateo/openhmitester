# -------------------------------------------------
# Project created by QtCreator 2009-01-22T09:35:51
# -------------------------------------------------


equals(QT_MAJOR_VERSION, 5) {
  message(" ================ QT 5 ================ ")
   QT +=
   CONFIG +=
}
else {
  message(" ================ QT 4 ================ ")
   QT +=
   CONFIG +=
}

CONFIG += debug

TARGET = OHTPreload
TEMPLATE = lib

DEFINES += LIBPRELOAD_LIBRARY


###
### OHT LibPreload sources
###

include(lib_preload.pri)

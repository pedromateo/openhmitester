# -------------------------------------------------
# Project created by QtCreator 2009-01-22T09:35:51
# -------------------------------------------------


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


QT += network xml
CONFIG += debug

TARGET = OHTPreload
TEMPLATE = lib

DEFINES += LIBPRELOAD_LIBRARY


###
### OHT LibPreload sources
###

include(lib_preload.pri)

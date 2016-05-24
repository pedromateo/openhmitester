# -------------------------------------------------
# Project created by QtCreator 2009-01-22T09:35:51
# -------------------------------------------------


equals(QT_MAJOR_VERSION, 5) {
   message("QMake lib_preload QT5")
   QT +=
   CONFIG +=
}
else {
   message("QMake lib_preload QT4")
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

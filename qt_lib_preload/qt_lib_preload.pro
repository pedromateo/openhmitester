# -------------------------------------------------
# Project created by QtCreator 2009-01-22T09:35:51
# -------------------------------------------------

#
# HMITester and OHTLibPreload common sources
#

include(../common/common.pri)

####
#### base Lib Preload project
####

include(../lib_preload/lib_preload.pri)


####
#### qt-linux Lib Preload project
####


equals(QT_MAJOR_VERSION, 5) {
  message(" ================ QT 5 ================ ")
   QT += widgets testlib
   CONFIG +=
}
else {
  message(" ================ QT 4 ================ ")
   QT +=
   CONFIG += qtestlib
}

QT += network xml
CONFIG += debug

TARGET = qt_oht_preload
TEMPLATE = lib


###
### OHT Qt LibPreload sources
###

include(qt_lib_preload.pri)


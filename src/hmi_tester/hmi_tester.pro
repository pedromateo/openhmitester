# -------------------------------------------------
# Project created by QtCreator 2008-12-12T12:28:02
# -------------------------------------------------


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

QT += script network
#CONFIG += debug

TARGET = HMITester
TEMPLATE = lib



#
# HMITester and OHTLibPreload common sources
#

include(../common/common.pri)


###
### HMITester  sources
###

include(../hmi_tester/hmi_tester.pri)


# -------------------------------------------------
# Project created by QtCreator 2008-12-12T12:28:02
# -------------------------------------------------


equals(QT_MAJOR_VERSION, 5) {
   message("QMake hmi_tester QT5")
   QT += widgets testlib network
   CONFIG +=
}
else {
   message("QMake hmi_tester QT4")
   QT += network
   CONFIG += qtestlib
}


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




# -------------------------------------------------
# Project created by QtCreator 2008-12-12T12:28:02
# -------------------------------------------------

#
# HMITester and OHTLibPreload common sources
#

include(../../common/common.pri)

####
#### base HMI Tester project
####

include(../../hmi_tester/hmi_tester.pri)


####
#### qt-linux HMI Tester project
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

QT += script xml network
CONFIG += debug

TARGET = qt_linux_hmi_tester
TEMPLATE = app


###
### qt-linux main sources
###

SOURCES += main.cpp
HEADERS += qtlinux_ohtconfig.h

###
### linux preloading
###

INCLUDEPATH += ../../preloaders/
SOURCES += ../../preloaders/linuxpreloadingaction.cpp
HEADERS += ../../preloaders/linuxpreloadingaction.h

###
### datamodel adapters
###

INCLUDEPATH += ../../datamodel_adapters
SOURCES += ../../datamodel_adapters/xmldatamodeladapter.cpp
HEADERS += ../../datamodel_adapters/xmldatamodeladapter.h

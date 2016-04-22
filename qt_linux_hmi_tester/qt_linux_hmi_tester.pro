# -------------------------------------------------
# Project created by QtCreator 2008-12-12T12:28:02
# -------------------------------------------------

#
# HMITester and OHTLibPreload common sources
#

include(../common/common.pri)

####
#### base HMI Tester project
####

include(../hmi_tester/hmi_tester.pri)


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
### qt-linux adaptation sources
###

SOURCES += main.cpp \
           linuxpreloadingaction.cpp

HEADERS += linuxpreloadingaction.h \
           qtlinux_ohtconfig.h


###
### datamodel sources
###

SOURCES += xmldatamodeladapter.cpp
HEADERS += xmldatamodeladapter.h

LIBS += -L/opt/boost/boost_1_60_0/lib/
INCLUDEPATH += /opt/boost/boost_1_60_0/include/



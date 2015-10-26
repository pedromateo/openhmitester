# -------------------------------------------------
# Project created by QtCreator 2008-12-12T12:28:02
# -------------------------------------------------


####
#### base HMI Tester project
####

include(../hmi_tester/hmi_tester.pri)

####
#### qt-linux HMI Tester project
####

equals(QT_MAJOR_VERSION, 5) {
  message(" ================ QT 5 ================ ")
   QT += widgets testlib xml network
   CONFIG += debug
}
else {
  message(" ================ QT 4 ================ ")
   QT += script xml network
   CONFIG += qtestlib debug
}

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



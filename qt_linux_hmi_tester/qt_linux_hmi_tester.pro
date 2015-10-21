# -------------------------------------------------
# Project created by QtCreator 2008-12-12T12:28:02
# -------------------------------------------------


####
#### base HMI Tester project
####

INCLUDEPATH += ../hmi_tester/
DEPENDPATH += ../hmi_tester/

include(../hmi_tester/hmi_tester.pro)

####
#### qt-linux HMI Tester project
####

QT += script xml network
CONFIG += qtestlib debug

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



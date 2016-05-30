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
   message("QMake qt_win_hmi_tester QT5")
   QT += widgets testlib
   CONFIG +=
}
else {
   message("QMake qt_win_hmi_tester QT4")
   QT +=
   CONFIG += qtestlib
}

QT += script xml network
CONFIG += debug

TARGET = qt_win_hmi_tester
TEMPLATE = app


###
### qt-linux main sources
###

SOURCES += main.cpp
linux {
   HEADERS += qtlinux_ohtconfig.h
}
win32 {
   HEADERS += qtwin_ohtconfig.h
}

###
### linux preloading
###

INCLUDEPATH += ../../preloaders/
SOURCES += ../../preloaders/winpreloadingaction.cpp
HEADERS += ../../preloaders/winpreloadingaction.h

###
### datamodel adapters
###

INCLUDEPATH += ../../datamodel_adapters/
SOURCES += ../../datamodel_adapters/xmldatamodeladapter.cpp
HEADERS += ../../datamodel_adapters/xmldatamodeladapter.h


###
### lib includes
###

# add here your boost includes if needed
linux {
LIBS += -L/opt/boost/boost_1_60_0/lib/
INCLUDEPATH += /opt/boost/boost_1_60_0/include/
}
win32 {
LIBS += -LC:\boost_1_60_0\stage\lib
INCLUDEPATH += C:\boost_1_60_0\boost\  C:\boost_1_60_0\
}



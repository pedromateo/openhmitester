#-------------------------------------------------
#
# Project created by QtCreator 2010-03-10T08:55:24
#
#-------------------------------------------------

TARGET = simusaes
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    nativecontroller.cpp

HEADERS  += mainwindow.h \
    nativecontroller.h

FORMS    += mainwindow.ui

OTHER_FILES += behavior/initialization.lua \
    behavior/proto_behavior.lua \
    behavior/alternative_design.lua \
    layer.config

LIBS += -lboost_system -lboost_program_options

#INCLUDEPATH += ../../../libs/current/ \
#    /usr/include/lua5.1/ \
#    /usr/local/include/luabind/

#LIBS += -llua5.1 \
#    -lluabindd \
#    -L/usr/local/lib/


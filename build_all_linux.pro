# -*- mode: c++; c-basic-offset: 4; c-basic-style: bsd; -*-
#/*
# *   This program is free software; you can redistribute it and/or
# *   modify
# *   it under the terms of the GNU Lesser General Public License as
# *   published by the Free Software Foundation; either version 3.0 of
# *   the License, or (at your option) any later version.
# *
# *   This program is distributed in the hope that it will be useful,
# *   but WITHOUT ANY WARRANTY; without even the implied warranty of
# *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# *   GNU Lesser General Public License for more details.
# *
# *   You should have received a copy of the GNU Lesser General Public
# *   License along with this library; if not, write to the Free Software
# *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
# *   02111-1307 USA
# *
# *   This file is part of the Open-HMI Tester,
# *   http://www.catedrasaes.org/wiki/OHT
# *
# */

# main target
TEMPLATE = subdirs
BUILD_DIR = ../openhmitester_build
DESTDIR = $${BUILD_DIR}
MOC_DIR = $${BUILD_DIR}/mocs
OBJECTS_DIR = $${BUILD_DIR}/objs

# config
CONFIG += no_keywords

# targets
SUBDIRS += src/common
src/common.file = src/common/common.pri
SUBDIRS += src/hmi_tester
SUBDIRS += src/lib_preload
SUBDIRS += src/qt_lib_preload
SUBDIRS += src/linux/qt_linux_hmi_tester
SUBDIRS += src/linux/qt_linux_lib_preload

# testbench
SUBDIRS += testbench/desktop
testbench/desktop.file = testbench/desktop/simusaes.pro
SUBDIRS += testbench/web
testbench/web.file = testbench/web/fancybrowser.pro



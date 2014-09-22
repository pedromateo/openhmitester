// -*- mode: c++; c-basic-offset: 4; c-basic-style: bsd; -*-
/*
 *   This program is free software; you can redistribute it and/or
 *   modify
 *   it under the terms of the GNU Lesser General Public License as
 *   published by the Free Software Foundation; either version 3.0 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *   02111-1307 USA
 *
 *   This file is part of the Open-HMI Tester,
 *   http://openhmitester.sourceforge.net
 *
 */
#ifndef OHTCONFIG_H
#define OHTCONFIG_H

/// ///
///
/// Configuration file
///
/// ///

///
///compilation config
///

/// windowing system
#define QT_ENABLED 1

/// DataModel Adapter
#define DA_XML_ENABLED 1
#define DA_XML "XML"

/// Preloading Action
#define PA_LINUX_ENABLED 1

///
/// SO config
///
#define PATH_SEPARATOR "/"

///
///Comm configuration
///
#define SERVER_IP "127.0.0.1"
#define TCP_PORT 1984

///
///Preload configuration
///
#define LIB_PRELOAD "libPreload/libLibPreload.so.1.0.0"

///
///Example values
///
#define EXAMPLE_VALUES_ENABLED 0
#define EXAMPLE_BINARY "/tmp/test"
#define EXAMPLE_PATH "/tmp/testsuite"
#define STANDARD_OUTPUT_FILE "/tmp/salida.txt"
#define ERROR_OUTPUT_FILE "/tmp/error.txt"

#endif // OHTCONFIG_H

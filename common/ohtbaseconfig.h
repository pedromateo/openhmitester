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
/// SO config
///
#define PATH_SEPARATOR "/"

///
///Comm configuration
///

#define SERVER_IP "127.0.0.1"
#define TCP_PORT 7357

///
/// hmi tester app configuration
///

#define OHT_FILE_EXTENSION "oht"

#define IDLE_OPACITY 1.0
#define RUNNING_OPACITY 0.5

///
/// playback process
///

#define EXEC_PAUSE_AFTER_REPLAY 2000

///
/// output files
///

#define STANDARD_OUTPUT_FILE "./testing_lib_out.log"
#define ERROR_OUTPUT_FILE "./testing_lib_error.log"

///
/// qsettings strings
///

#define SETT_LAST_OPEN_DIR "lastOpenDir"
#define SETT_LAST_AUT_DIR "lastAutDirectory"
#define SETT_LAST_SAVE_DIR "lastSaveDir"

#endif // OHTCONFIG_H

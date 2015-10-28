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
#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <cassert>

///
/// custom debug
///

#define DEBUG_ENABLED 1

#define DEBUGc(content)\
if (DEBUG_ENABLED) std::cout << content << std::endl;

///
/// parameterized debug
///

// Debug types enabled are values > 0
// Debug types disabled are values == 0
// Debug types enabled should have different values than D_ERROR, therefore
// the OHT framework can differentiate between errors and other debug outputs

#define D_ERROR 1
#define D_PLAYBACK 2
#define D_RECORDING 2
#define D_BOTH 2
#define D_PRELOAD 3
#define D_EXECUTOR 0
#define D_CONSUMER 3
#define D_GUI 4
#define D_COMM 5

//method
#define DEBUG(type,content)\
if (type == D_ERROR) std::cerr << content << std::endl;\
else if (type && DEBUG_ENABLED) std::cout << content << std::endl



#endif // DEBUG_H

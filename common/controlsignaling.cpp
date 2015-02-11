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

#include "controlsignaling.h"

using namespace Control;

///
/// control test item base
///

//constructor
ControlTestItem::ControlTestItem()
{
    type(CTI_TYPE);
}


///
///  error
///

//constructor
CTI_Error::CTI_Error()
{
    subtype(CTI_ERROR);
}

//error message
void CTI_Error::description(const std::string& text)
{
    addData(CTI_Error_Description,text);
}

const std::string CTI_Error::description()
{
    return getData(CTI_Error_Description);
}


///
/// StartPlayback
///

//constructor
CTI_StartPlayback::CTI_StartPlayback()
{
    subtype(CTI_START_PLAYBACK);
}


///
/// StopPlayback
///

//constructor
CTI_StopPlayback::CTI_StopPlayback()
{
    subtype(CTI_STOP_PLAYBACK);
}

///
/// PausePlayback
///

//constructor
CTI_PausePlayback::CTI_PausePlayback()
{
    subtype(CTI_PAUSE_PLAYBACK);
}


///
/// StartRecording
///

//constructor
CTI_StartRecording::CTI_StartRecording()
{
    subtype(CTI_START_RECORDING);
}


///
/// StopRecording
///

//constructor
CTI_StopRecording::CTI_StopRecording()
{
    subtype(CTI_STOP_RECORDING);
}

///
/// PauseRecording
///

//constructor
CTI_PauseRecording::CTI_PauseRecording()
{
    subtype(CTI_PAUSE_RECORDING);
}


///
/// EventExecuted
///

//constructor
CTI_EventExecuted::CTI_EventExecuted()
{
    subtype(CTI_EVENT_EXECUTED);
}


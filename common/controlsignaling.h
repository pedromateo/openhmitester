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
#ifndef CONTROLSIGNALING_H
#define CONTROLSIGNALING_H

#include <datamodel.h>

namespace Control
{

    ///
    /// constants
    ///

    const int CTI_TYPE = -1;
    // 0 -> control signaling OHT > PM
    const int CTI_ERROR = 0;
    // 90 -> control signaling PM > OHT
    const int CTI_EVENT_EXECUTED = 91;
    //
    //
    // 10 -> playback
    const int CTI_START_PLAYBACK = 11;
    const int CTI_STOP_PLAYBACK = 12;
    const int CTI_PAUSE_PLAYBACK = 13;
    // 20 -> recording
    const int CTI_START_RECORDING = 21;
    const int CTI_STOP_RECORDING = 22;
    const int CTI_PAUSE_RECORDING = 23;

    ///
    /// control test item base
    ///
    class ControlTestItem : public DataModel::TestItem
    {
    public:
        //constructor
        ControlTestItem();
    };

    ///
    ///  error
    ///
    const std::string CTI_Error_Description = "desc";
    class CTI_Error : public ControlTestItem
    {
    public:
        //constructor
        CTI_Error();

        //error message
        void description(const std::string&);
        const std::string description();

    };

    ///
    /// StartPlayback
    ///
    class CTI_StartPlayback : public ControlTestItem
    {
    public:
        //constructor
        CTI_StartPlayback();
    };

    ///
    /// StopPlayback
    ///
    class CTI_StopPlayback : public ControlTestItem
    {
    public:
        //constructor
        CTI_StopPlayback();
    };

    ///
    /// PausePlayback
    ///
    class CTI_PausePlayback : public ControlTestItem
    {
    public:
        //constructor
        CTI_PausePlayback();
    };

    ///
    /// StartRecording
    ///
    class CTI_StartRecording : public ControlTestItem
    {
    public:
        //constructor
        CTI_StartRecording();
    };

    ///
    /// StopRecording
    ///
    class CTI_StopRecording : public ControlTestItem
    {
    public:
        //constructor
        CTI_StopRecording();
    };

    ///
    /// PauseRecording
    ///
    class CTI_PauseRecording : public ControlTestItem
    {
    public:
        //constructor
        CTI_PauseRecording();
    };

    ///
    /// EventExecuted
    ///
    class CTI_EventExecuted : public ControlTestItem
    {
    public:
        //constructor
        CTI_EventExecuted();
    };


}

#endif // CONTROLSIGNALING_H

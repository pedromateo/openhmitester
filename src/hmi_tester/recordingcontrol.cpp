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

#include "recordingcontrol.h"

RecordingControl::RecordingControl(Comm* c, RecordingObserver* ro)
        : comm_ (c), observer_ (ro)
{
    //item manager to this
    itemManager_.reset (new ItemManager(comm_, observer_));
}

RecordingControl::~RecordingControl()
{
}


/// ///
///
/// recording process control
///
/// ///
void RecordingControl::recordTestCase(DataModel::TestCase* tc)
{
    itemManager_->recordTestCase(tc);
}

void RecordingControl::pauseRecording()
{
    itemManager_->pauseRecording();
}

void RecordingControl::resumeRecording()
{
    itemManager_->resumeRecording();
}

void RecordingControl::stopRecording()
{
    itemManager_->stopRecording();
}

void RecordingControl::applicationFinished()
{
    itemManager_->applicationFinished();
}



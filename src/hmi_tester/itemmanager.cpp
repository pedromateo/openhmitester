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

#include "itemmanager.h"
#include <datamodel.h>
#include <datamodelmanager.h>
#include <controlsignaling.h>
#include <debug.h>

/// ///
///
/// constructor
///
/// ///
ItemManager::ItemManager(Comm* c, RecordingObserver* ro)
    : comm_(c), observer_(ro)
{
    //signals from comm
    connect(comm_, SIGNAL(receivedTestItem (DataModel::TestItem*)),
            this, SLOT(handleNewTestItem ( DataModel::TestItem* )));

    //flags initialization
    f_recording_ = false;
    f_paused_ = false;
    f_terminate_ = false;
    f_error_ = false;

    //current test case
    currentTestCase_ = NULL;
}
ItemManager::~ItemManager()
{
}


/// ///
///
/// recording process control
///
/// ///
void ItemManager::recordTestCase(DataModel::TestCase* tc)
{
    DEBUG(D_RECORDING, "(ItemManager::recordTestCase)");
    //current test case reference
    assert(tc);
    currentTestCase_ = tc;

    //updating flags
    f_recording_ = true;
    f_paused_ = false;
    f_terminate_ = false;
    f_error_ = false;

    //reseting counter
    rtiCounter_ = 0;

    DEBUG(D_RECORDING,"(ItemManager::recordTestCase) Flags updated.");

    //sending "START RECORDING COMMAND"
    Control::CTI_StartRecording cti;
    comm_->handleSendTestItem(cti);

    DEBUG(D_RECORDING,"(ItemManager::recordTestCase) CTI_StartRecording posted.");
}

void ItemManager::pauseRecording()
{
    DEBUG(D_RECORDING, "(ItemManager::pauseRecording)");
    f_paused_ = true;
    f_recording_ = false;

    //sending "STOP RECORDING COMMAND"
    Control::CTI_StopRecording cti;
    comm_->handleSendTestItem(cti);
}

void ItemManager::resumeRecording()
{
    DEBUG(D_RECORDING, "(ItemManager::resumeRecording)");
    f_paused_ = true;
    f_recording_ = false;

    //sending "START RECORDING COMMAND"
    Control::CTI_StartRecording cti;
    comm_->handleSendTestItem(cti);
}

void ItemManager::stopRecording()
{

    DEBUG(D_RECORDING, "(ItemManager::stopRecording)");
    //updating flags and pointer
    f_terminate_ = true;
    f_recording_ = false;
    f_paused_ = false;
    f_error_ = false;

    //reseting counter
    rtiCounter_ = 0;

    //sending "STOP RECORDING COMMAND"
    Control::CTI_StopRecording cti;
    comm_->handleSendTestItem(cti);

    //emiting the recording process finished signal
    observer_->testRecordingFinished(currentTestCase_);

}

void ItemManager::applicationFinished()
{
    //if the process is recording
    if (f_recording_)
    {
        DEBUG(D_RECORDING, "(ItemManager::applicationFinished)");
        //updating flags and pointer
        f_terminate_ = true;
        f_recording_ = false;
        f_paused_ = false;
        f_error_ = false;

        //reseting counter
        rtiCounter_ = 0;

        //emiting the recording process finished signal
        observer_->testRecordingFinished(currentTestCase_);
    }
}

/// ///
///
/// recording process state
///
/// ///

bool ItemManager::isRecording()
{
    return f_recording_;
}
bool ItemManager::isPaused()
{
    return f_paused_;
}

/// ///
///
/// messages received from Preload Module
///
/// ///
void ItemManager::handleNewTestItem (DataModel::TestItem* ti)
{
    DEBUG(D_RECORDING, "(ItemManager::handleNewTestItem)");
    //if it is in recording process...
    if (isRecording() && currentTestCase_)
    {
        DEBUG(D_RECORDING, "(ItemManager::handleNewTestItem) Adding new TestItem to the current TestCase.");
        DEBUG(D_RECORDING, "(ItemManager::handleNewTestItem) TestItem type = " << ti->type());
        //add the to a new TestItem
        currentTestCase_->addTestItem(ti);
        //updating counter
        rtiCounter_++;
        //emiting rtiCounter signal
        observer_->testItemsReceivedCounter(rtiCounter_);
    }
}


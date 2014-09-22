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

#include "preloadcontroller.h"
#include <ohtconfig.h>
#include <debug.h>
#include "Qt_Imp/qteventconsumer.h"
#include "Qt_Imp/qteventexecutor.h"
#include <cassert>

PreloadController::PreloadController(PreloadingControl*)
{
}

///
///init method
///
bool PreloadController::initialize()
{
    //create comm
    comm_.reset (new Comm(TCP_PORT, false));

    //create and install consumer
    if (QT_ENABLED)
    {
        eConsumer_.reset(new QtEventConsumer());
        eConsumer_->install();
    }
    //TODO add new consumers here!!!
    assert(eConsumer_.get());

    //create and install executor
    if (QT_ENABLED)
    {
        eExecutor_.reset(new QtEventExecutor());
        eExecutor_->install();
    }
    //TODO add new executors here!!!
    assert(eExecutor_.get());

    ///
    /// signals connection
    ///

    //signals between comm and preloadController
    connect(this, SIGNAL(sendTestItem(const DataModel::TestItem&)),
            comm_.get(), SLOT(handleSendTestItem (const DataModel::TestItem&)));
    connect(comm_.get(), SIGNAL(receivedTestItem (DataModel::TestItem*)),
            this, SLOT(handleReceivedTestItem (DataModel::TestItem*)));

    //signals between eventConsumer and comm
    connect(eConsumer_.get(), SIGNAL(newTestItem(const DataModel::TestItem&)),
            comm_.get(), SLOT(handleSendTestItem (const DataModel::TestItem&)));

    ///
    /// process state control
    ///
    state_ = STOP;

    return true;
}

///
///state info
///
PreloadController::ProcessState PreloadController::state() const
{
    return state_;
}

///
///input method (comm signal handle)
///
void PreloadController::handleReceivedTestItem (DataModel::TestItem* ti)
{
    DEBUG(D_PRELOAD, "(PreloadController::handleReceivedTestItem)");
    //if it is a control item...
    if (ti->type() == Control::CTI_TYPE)
    {
        handleReceivedControl(static_cast<Control::ControlTestItem*>(ti));
        DEBUG(D_PRELOAD, "(PreloadController::handleReceivedTestItem) Control event handled.");
    }
    //if not...
    else
    {
        //if play process is enabled redirect the event
        if (state() == PLAY)
        {
            eExecutor_->handleNewTestItemReceived(ti);
            //TODO delete the item here????
            DEBUG(D_PRELOAD, "(PreloadController::handleReceivedTestItem) Event handled.");

            //and send a control event to synchronize the process
            Control::CTI_EventExecuted cti;
            comm_->handleSendTestItem(cti);
            DEBUG(D_PRELOAD, "(PreloadController::handleReceivedTestItem) Event executed notified.");
        }
    }
}

///
///input method (control signaling)
///
void PreloadController::handleReceivedControl (Control::ControlTestItem* cti)
{
    DEBUG(D_PRELOAD, "(PreloadController::handleReceivedControl)");
    ///
    ///depending on the subtype...
    ///

    // 10 -> playback
    //const int CTI_START_PLAYBACK = 11;
    if (cti->subtype() == Control::CTI_START_PLAYBACK)
    {
        state_ = PLAY;
        DEBUG(D_PRELOAD, "(PreloadController::handleReceivedControl) STATE: Start playback.");
        execution_start();
    }
    //const int CTI_STOP_PLAYBACK = 12;
    else if (cti->subtype() == Control::CTI_STOP_PLAYBACK)
    {
        state_ = STOP;
        DEBUG(D_PRELOAD, "(PreloadController::handleReceivedControl) STATE: Stop playback.");
        execution_stop();
    }
    //const int CTI_PAUSE_PLAYBACK = 13;
    else if (cti->subtype() == Control::CTI_PAUSE_PLAYBACK)
    {
        state_ = PAUSE_PLAY;
        DEBUG(D_PRELOAD, "(PreloadController::handleReceivedControl) STATE: Pause playback.");
        execution_pause();
    }
    // 20 -> recording
    //const int CTI_START_RECORDING = 21;
    else if (cti->subtype() == Control::CTI_START_RECORDING)
    {
        state_ = RECORD;
        DEBUG(D_PRELOAD, "(PreloadController::handleReceivedControl) STATE: Start recording.");
        capture_start();
    }
    //const int CTI_STOP_RECORDING = 22;
    else if (cti->subtype() == Control::CTI_STOP_RECORDING)
    {
        state_ = STOP;
        DEBUG(D_PRELOAD, "(PreloadController::handleReceivedControl) STATE: Stop recording.");
        capture_stop();
    }
    //const int CTI_PAUSE_RECORDING = 23;
    else if (cti->subtype() == Control::CTI_PAUSE_RECORDING)
    {
        state_ = PAUSE_RECORD;
        DEBUG(D_PRELOAD, "(PreloadController::handleReceivedControl) STATE: Pause recording.");
        capture_pause();
    }
}



///
///processes control
///
void PreloadController::capture_start()
{
    eConsumer_->startCapture();
}

void PreloadController::capture_pause()
{
    eConsumer_->pauseCapture();
}

void PreloadController::capture_stop()
{
    eConsumer_->stopCapture();
}

void PreloadController::execution_start()
{
    eExecutor_->startExecution();
}

void PreloadController::execution_pause()
{
    eExecutor_->pauseExecution();
}

void PreloadController::execution_stop()
{
    eExecutor_->stopExecution();
}


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
#include <ohtbaseconfig.h>
#include <debug.h>
#include <cassert>

PreloadController::PreloadController(PreloadingControl* pc, EventConsumer *ec, EventExecutor *ex)
{
    _ev_consumer = ec;
    _ev_executor = ex;
}

PreloadController::~PreloadController()
{
    if (_comm != NULL){
        _comm->stop();
        delete _comm;
    }
}

///
///init method
///
bool PreloadController::initialize()
{
    //create and start comm
    _comm = new Comm(TCP_PORT, false);
    _comm->resetAndStart();

    //install consumer
    _ev_consumer->install();

    //install executor
    _ev_executor->install();

    ///
    /// signals connection
    ///

    //signals between comm and preloadController
    connect(this, SIGNAL(sendTestItem(const DataModel::TestItem&)),
            _comm, SLOT(handleSendTestItem (const DataModel::TestItem&)));
    connect(_comm, SIGNAL(receivedTestItem (DataModel::TestItem*)),
            this, SLOT(handleReceivedTestItem (DataModel::TestItem*)));

    //signals between eventConsumer and comm
    connect(_ev_consumer, SIGNAL(newTestItem(const DataModel::TestItem&)),
            _comm, SLOT(handleSendTestItem(const DataModel::TestItem&)));

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
            _ev_executor->handleNewTestItemReceived(ti);
            //TODO delete the item here????
            DEBUG(D_PRELOAD, "(PreloadController::handleReceivedTestItem) Event handled. Type = "
                  << ti->type() << " Subtype = " << ti->subtype());

            //and send a control event to synchronize the process
            Control::CTI_EventExecuted cti;
            _comm->handleSendTestItem(cti);
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
    _ev_consumer->startCapture();
}

void PreloadController::capture_pause()
{
    _ev_consumer->pauseCapture();
}

void PreloadController::capture_stop()
{
    _ev_consumer->stopCapture();
}

void PreloadController::execution_start()
{
    _ev_executor->startExecution();
}

void PreloadController::execution_pause()
{
    _ev_executor->pauseExecution();
}

void PreloadController::execution_stop()
{
    _ev_executor->stopExecution();
}


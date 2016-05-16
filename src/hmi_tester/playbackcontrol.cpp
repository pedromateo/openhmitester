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

#include "playbackcontrol.h"
#include "executionthread.h"
#include "debug.h"
#include <boost/ref.hpp>

PlaybackControl::PlaybackControl(Comm *c, PlaybackObserver* pc)
    : comm_ (c), observer_ (pc)
{
}

PlaybackControl::~PlaybackControl()
{
}


/// ///
///
/// execution process control
///
/// ///
bool PlaybackControl::runTestCase(DataModel::TestCase* tc, float speed)
{
    // //execution thread should exist and be stoped
    // if (!executionThread_.get())
    // {
    //     DEBUG(D_ERROR, "(PlaybackControl::runTestCase) ERROR: Execution Thread is NULL.");
    //     return false;
    // }

    // if (executionThread_->isRunning())
    // {
    //     DEBUG(D_ERROR, "(PlaybackControl::runTestCase) ERROR: Execution Thread is running.");
    //     return false;
    // }

    // create a new execution thread for this testcase
    executionThread_.reset(new ExecutionThread (comm_, observer_, speed));


    //execution thread
    assert(tc);
    executionThread_->currentTestCase (tc);
    _internal_thread = boost::thread (boost::ref (*(executionThread_.get())));

    return true;
}


bool PlaybackControl::pauseExecution()
{
    //it should be running
    if (executionThread_->isRunning())
    {
        executionThread_->pause();
        return true;
    }
    return false;
}

bool PlaybackControl::resumeExecution()
{
    //it should be paused
    if (executionThread_->isPaused())
    {
        executionThread_->resume();
        return true;
    }
    return false;
}

bool PlaybackControl::stopExecution()
{
    //it should be running or paused
    if (executionThread_->isRunning()
            || executionThread_->isPaused())
    {
        DEBUG(D_PLAYBACK, "(PlaybackControl::stopExecution)");
        executionThread_->stop();
        // Wait for it to finish
        _internal_thread.join();
        return true;
    }
    return false;
}

///
/// some notification signal handlers
///
void PlaybackControl::applicationFinished()
{
    //it should be running or paused
    if (executionThread_->isRunning()
            || executionThread_->isPaused())
    {
        executionThread_->applicationFinished();

        // Wait the thread
        _internal_thread.join();
    }
}

void PlaybackControl::handleEventExecutedOnPreloadModule()
{
    //add a new item on the execution semaphore
    executionThread_->continueExecution();
}

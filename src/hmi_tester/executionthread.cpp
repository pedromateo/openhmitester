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
/*
TODO in future work:
- robustness timer
- no-pause conditions (mouse release)
*/

#include "executionthread.h"
#include <datamodelmanager.h>
#include <controlsignaling.h>
#include <debug.h>

#include <cassert>
#include <string>
#include <boost/lexical_cast.hpp>

#include <ohtbaseconfig.h>


///
/// constructors
///
ExecutionThread::ExecutionThread(Comm *c, PlaybackObserver* pc , float speed)
    : _comm (c), _observer (pc), _executionSpeed(speed)
{
    // flags
    threadState_ = NONE;
    currentTestCase_ = NULL;
}

ExecutionThread::~ExecutionThread()
{}

///
/// Execution method (thread)
///
void ExecutionThread::operator()()
{
    DEBUG(D_PLAYBACK, "(ExecutionThread::run)");

    ///reference checking
    assert(_comm);
    assert(currentTestCase_);

    /// test case checking
    if (!currentTestCase_)
    {
        DEBUG(D_ERROR, "(ExecutionThread::run) No testcase to play.");
        threadState_= ERROR;
        return;
    }

    DEBUG(D_PLAYBACK, "(ExecutionThread::run) TestCase " <<
          currentTestCase_->name() << " OK.");

    ///flags initialization (in running state)
    threadState_ = RUN;
    pendingState_ = NONE;

    ///
    /// start testcase execution
    ///

    // Send start playback and wait for ACK.
    _sendStartPlayback();

    // set counters
    int total = currentTestCase_->count();
    int counter = 0;
    DataModel::TestCase::TestItemList::const_iterator it;
    const DataModel::TestCase::TestItemList& il =
            currentTestCase_->testItemList();

    /// for each testItem at the list...
    for (it = il.begin(); it != il.end(); ++it)
    {
        DataModel::TestItem& ti = const_cast<DataModel::TestItem&>(*it);

        //counter control
        counter++;

        // modifying sleep time according to selected speed
        ti.timestamp(ti.timestamp() / _executionSpeed);

        //sending test item to preload module
        _comm->handleSendTestItem(ti);
        DEBUG(D_PLAYBACK, "(ExecutionThread::run) Item sent.");

        //debug
        DEBUG(D_PLAYBACK, "(ExecutionThread::run) Executed testItem " <<
              counter <<
              " of "  <<
              currentTestCase_->count() <<
              ".");

        //completed percentage notification
        _observer->completedPercentageNotification(counter * 100.0 / total);

        //wait before continuing with the next test
        waitExecution();

        DEBUG(D_PLAYBACK, "(ExecutionThread::run) Continuing execution.");

        // Once the command executed, attend pending PAUSE or STOP
        if (pendingState_ == PAUSED)
        {
            //sending "PAUSE PLAYBACK COMMAND"
            _comm->handleSendTestItem(Control::CTI_PausePlayback());
            threadState_  = PAUSED;
            pendingState_ = NONE;

            // lock the mutex
            boost::unique_lock<boost::mutex> lock(pause_mutex_);
            resume_pause_.wait(lock);
            DEBUG(D_PLAYBACK, "(ExecutionThread::run) Pause Mutex unlocked.");
        } else if (pendingState_ == STOPPED)
        {
            threadState_ = STOPPED;
            pendingState_ = NONE;
            break; // exit

        } else if (threadState_ == WANT_TERMINATE)
        {
            break; // exit
        }

    } // for

    // pause after replay
    _sleep(EXEC_PAUSE_AFTER_REPLAY);

    ///
    /// finish testcase execution
    ///

    DEBUG(D_PLAYBACK, "(ExecutionThread::run) Finishing thread.");

    // FIXME: Send this only if threadState_ != WANT_TERMINATE
    _sendStopPlayback();

    DEBUG(D_PLAYBACK, "(ExecutionThread::run) StopPlayback command sent.");


    DEBUG(D_PLAYBACK, "(ExecutionThread::run) Finishing."
          "________________________________________________________________");

    //execution thread finished notification
    int result = 0;
    if (threadState_ == WANT_TERMINATE) result = 2;
    else if (threadState_ == ERROR) result = 1;
    _observer->executionThreadTerminated(result);

    ///
    ///
    ///ending actions and memory freeing
    threadState_ = NONE;
    pendingState_ = NONE;
    currentTestCase_ = NULL;

    return;
}

void ExecutionThread::_sendStartPlayback()
{
    DEBUG(D_PLAYBACK, "(ExecutionThread::run) Sending START PLAYBACK COMMAND");
    //sending "START PLAYBACK COMMAND"
    Control::CTI_StartPlayback cti;
    _comm->handleSendTestItem(cti);

    // Wait for execution
    //waitExecution();
}

void ExecutionThread::_sendStopPlayback()
{
    DEBUG(D_PLAYBACK, "(ExecutionThread::run) Sending STOP PLAYBACK COMMAND");
    //sending "STOP PLAYBACK COMMAND"
    Control::CTI_StopPlayback cti2;
    _comm->handleSendTestItem(cti2);

    // Wait for execution
    //waitExecution();
}


void ExecutionThread::_sleep(int ms)
{
    boost::this_thread::sleep(
                boost::posix_time::milliseconds(ms));
}

///
/// pauses the process
///
void ExecutionThread::pause()
{
    DEBUG(D_PLAYBACK, "(ExecutionThread::pause)");

    // Signal the future wanted state
    if (threadState_ == RUN)
        pendingState_ = PAUSED;
}

///
/// resumes the process
///
void ExecutionThread::resume()
{
    DEBUG(D_PLAYBACK, "(ExecutionThread::resume)");

    // release the mutex, only if paused
    if (threadState_ == PAUSED)
    {
        threadState_ = RUN;

        //sending "START PLAYBACK COMMAND"
        Control::CTI_StartPlayback cti;
        _comm->handleSendTestItem(cti);

        // notify resume
        resume_pause_.notify_all();

        pendingState_ = NONE;
    }
    // FIXME: else throw?
}

///
/// stop the thread
///
// FIXME: by  now stop == kill, as the thread is never killed really.
// a better separation between kill and stop must be done.
void ExecutionThread::stop()
{
    DEBUG(D_PLAYBACK, "(ExecutionThread::stop)");

    pendingState_ = STOPPED;
    continueExecution();
}

///
/// kill the thread
///
void ExecutionThread::kill()
{
    DEBUG(D_PLAYBACK, "(ExecutionThread::kill)");
    //kill the thread and wait for it
    pendingState_ = STOPPED;
    continueExecution();
}

///
/// tested application finished notification
///
void ExecutionThread::applicationFinished()
{
    DEBUG(D_PLAYBACK, "(ExecutionThread::applicationFinished)");
    stop();
}

///
/// true if it is running
///
bool ExecutionThread::isRunning()
{
    return threadState_ == RUN;
}

///
/// true if it is paused
///
bool ExecutionThread::isPaused()
{
    return threadState_ == PAUSED;
}


///
/// //test case execution
///


///
///
///
void ExecutionThread::currentTestCase(DataModel::TestCase* tc)
{
    DEBUG(D_PLAYBACK, "(ExecutionThread::currentTestCase)");
    assert(tc);
    currentTestCase_ = tc;
}


///
/// //execution semaphore -> execution flow control
///


///
/// add one ticket to the semaphore
///
void ExecutionThread::continueExecution()
{
    DEBUG(D_PLAYBACK, "(ExecutionThread::continueExecution)");
    next_step_ready_.notify_all();
    DEBUG(D_PLAYBACK, "(ExecutionThread::continueExecution) Exit.");
}

///
/// acquires one ticket from the semaphore
///
void ExecutionThread::waitExecution()
{
    boost::unique_lock<boost::mutex> lock(step_mutex_);

    DEBUG(D_PLAYBACK, "(ExecutionThread::waitExecution)");
    next_step_ready_.wait(lock);
    DEBUG(D_PLAYBACK, "(ExecutionThread::waitExecution) Exit.");
}

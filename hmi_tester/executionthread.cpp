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
#include "datamodelmanager.h"
#include "controlsignaling.h"
#include "debug.h"

#include <cassert>
#include <string>
#include <boost/lexical_cast.hpp>

///
/// constants
///
const int END_BIN_WAIT = 3000;
const int EXEC_SPEED_PAUSE = 150;
const int EXEC_SPEED_BASE = 10;

///
/// constructors
///
ExecutionThread::ExecutionThread(Comm *c, ExecutionObserver* pc )
    : comm_ (c), observer_ (pc)
{
    //flags
    threadState_ = NONE;
    currentTestCase_ = NULL;

    changeExecutionSpeed(50);
}

ExecutionThread::~ExecutionThread()
{}

///
/// Execution method (thread)
///
void ExecutionThread::operator()()
{
    DEBUG(D_PLAYBACK, "(ExecutionThread::run)");
    ///references checking
    assert(comm_);
    assert(currentTestCase_);

    /// test case checking
    if (!currentTestCase_)
    {
	threadState_= ERROR;
	return;
    }

    DEBUG(D_PLAYBACK, "(ExecutionThread::run) TestCase " <<
	  currentTestCase_->name() << " OK.");

    ///flags initialization (in running state)
    threadState_ = RUN;
    pendingState_ = NONE;

    // Send start playback and wait for ACK.
    _sendStartPlayback();

    int total = currentTestCase_->count();
    int counter = 0;
    DataModel::TestCase::TestItemList::const_iterator it;
    const DataModel::TestCase::TestItemList& il =
	currentTestCase_->testItemList();

    /// for each testItem at the list...
    for (it = il.begin(); it != il.end(); ++it)
    {
        const DataModel::TestItem& ti = *it;

	//counter control
	counter++;

	//sending test item to preload module
	comm_->handleSendTestItem(ti);
	DEBUG(D_PLAYBACK, "(ExecutionThread::run) Item sent.");

	//debug
	DEBUG(D_PLAYBACK, "(ExecutionThread::run) Executed testItem " <<
	      counter <<
	      " of "  <<
	      currentTestCase_->count() <<
	      ".");

	//completed percentage notification
	observer_->completedPercentageNotification(counter * 100.0 / total);

	//wait before continuing with the next test
	waitExecution();

	DEBUG(D_PLAYBACK, "(ExecutionThread::run) Continuing execution.");

	// Once the command executed, attend pending PAUSE or STOP
	if (pendingState_ == PAUSED)
	{
	    //sending "PAUSE PLAYBACK COMMAND"
	    comm_->handleSendTestItem(Control::CTI_PausePlayback());
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
	} else
	{
	    //sleep in order to simulate execution speed
	    boost::this_thread::
		sleep (boost::posix_time::milliseconds( executionSpeed_ ));
	    DEBUG(D_PLAYBACK, "(ExecutionThread::run) Wait the execution speed.");
	}

    } // for

    DEBUG(D_PLAYBACK, "(ExecutionThread::run) Finishing thread.");

    // FIXME: Send this only if threadState_ != WANT_TERMINATE
    _sendStopPlayback();

    DEBUG(D_PLAYBACK, "(ExecutionThread::run) StopPlayback command sent.");

    //execution thread finished notification
    int result = 0;
    if (threadState_ == WANT_TERMINATE) result = 2;
    else if (threadState_ == ERROR) result = 1;
    observer_->executionThreadTerminated(result);

    ///
    ///
    ///ending actions and memory freeing
    threadState_ = NONE;
    pendingState_ = NONE;
    currentTestCase_ = NULL;

    DEBUG(D_PLAYBACK, "(ExecutionThread::run) Finished."
	  "________________________________________________________________");

    return;
}

void
ExecutionThread::_sendStartPlayback()
{
    //sending "START PLAYBACK COMMAND"
    Control::CTI_StartPlayback cti;
    comm_->handleSendTestItem(cti);

    // Wait for execution
    //waitExecution();
}

void
ExecutionThread::_sendStopPlayback()
{
    //sending "STOP PLAYBACK COMMAND"
    Control::CTI_StopPlayback cti2;
    comm_->handleSendTestItem(cti2);

    // Wait for execution
    //waitExecution();
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
	comm_->handleSendTestItem(cti);

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
/// //execution speed
///


///
/// changes execution speed
///
void ExecutionThread::changeExecutionSpeed(int speed)
{
    /*
      it might receive:
      - 100 - higher
      - 75
      - 50  - default
      - 25  - lower
    */
    double speedInverseValue = (1/(speed/25.0))*2;
    DEBUG(D_PLAYBACK, "(ExecutionThread::changeExecutionSpeed)");
    executionSpeed_ = EXEC_SPEED_BASE + speedInverseValue * EXEC_SPEED_PAUSE;
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

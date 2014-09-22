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
#ifndef EXECUTIONTHREAD_H
#define EXECUTIONTHREAD_H

#include "datamodel.h"
#include "comm.h"
#include "executionobserver.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

class ExecutionThread
{
    // Do not copy
    ExecutionThread (const ExecutionThread&);

    enum thread_state_t { NONE, PAUSED, RUN, STOPPED, WANT_TERMINATE, ERROR };

public:
    ExecutionThread(Comm*, ExecutionObserver* );
    ~ExecutionThread();

public:

    //execution control
    void pause();
    void resume();
    void stop();
    void kill();

    void applicationFinished();

    bool isRunning();
    bool isPaused();

    //test case execution
    void currentTestCase(DataModel::TestCase*);

    //execution speed
    void changeExecutionSpeed(int);

    //execution semaphore
    void continueExecution();
    void waitExecution();


public:
    void operator()();

private:

    //execution flags
    thread_state_t threadState_;
    thread_state_t pendingState_;

    //mutex -> for pause and resume
    boost::mutex pause_mutex_;
    boost::condition_variable resume_pause_;

    // Condition variable to step execution
    boost::mutex step_mutex_;
    boost::condition_variable next_step_ready_;

    //test case to be executed
    DataModel::TestCase *currentTestCase_;

    //comm module reference
    Comm *comm_;

    // Control object
    ExecutionObserver* observer_;

    //execution speed
    int executionSpeed_;

    void _sendStartPlayback();
    void _sendStopPlayback();
};


#endif // EXECUTIONTHREAD_H

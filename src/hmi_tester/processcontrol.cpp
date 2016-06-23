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

#include "processcontrol.h"
#include <controlsignaling.h>
#include <ohtbaseconfig.h>
#include <debug.h>
#include <qtutils.h>
#include <hmitestercontrol.h>

ProcessControl::ProcessControl(PreloadingAction *pa, DataModelAdapter *dma)
{
    //variable initialization
    gui_reference_ = NULL;
    current_filename_ = "";

    // store specific preloading action
    assert(pa);
    preloading_action_ = pa;

    // store specific datamodel adapter
    assert(dma);
    dataModel_adapter_ = dma;

    //state
    _setState(INIT);
}

ProcessControl::~ProcessControl()
{
}

/// ///
///
/// public methods
///
/// ///

void ProcessControl::initialize()
{
    ///
    /// state
    _setState(INIT);

    ///
    /// communication manager creation
    _comm.reset (new Comm(TCP_PORT, true));

    ///
    /// dataModelManager
    dataModel_manager_ = new DataModelManager();
    dataModel_manager_->addDataModelAdapter(dataModel_adapter_->id(), dataModel_adapter_);
    assert(dataModel_manager_->setCurrentDataModelAdapter(dataModel_adapter_->id()));

    ///
    /// checking preload library

    current_oht_path_ = QtUtils::getCurrentDir().toStdString() + PATH_SEPARATOR;
    current_libPreload_path_ = current_oht_path_ + preloading_action_->libPreloadPath();

    DEBUG(D_GUI,"(ProcessControl::initializeForm) Finding preload lib at: " << current_libPreload_path_);

    if (QtUtils::fileExists(QString(current_libPreload_path_.c_str()))){
        DEBUG(D_GUI,"(ProcessControl::initializeForm) Preload lib found.");
    }
    else
    {
        DEBUG(D_GUI,"(ProcessControl::initializeForm) Failed Preload lib checking.");
        DEBUG(D_GUI,"(ProcessControl::initializeForm) libpath = " << current_libPreload_path_);
        QtUtils::newErrorDialog ( QString("Preload Library not found at: ") + QString(current_libPreload_path_.c_str()));
        assert ( 0 );
    }

    assert(current_libPreload_path_.empty() == false);

    ///
    /// playback control creation
    playback_control_.reset (new PlaybackControl(_comm.get(), this));

    ///
    /// recording control creation
    recording_control_.reset(new RecordingControl(_comm.get(), this));

    ///
    /// signals connection

    //signals between this and Comm
    connect(_comm.get(),SIGNAL(error(const std::string&)),
            this,SLOT(slot_handleCommError(const std::string&)));
    connect(_comm.get(),SIGNAL(receivedTestItem (DataModel::TestItem*)),
            this,SLOT(handleControlSignaling (DataModel::TestItem*)));

    //signals between preloadingAction and this
    connect(preloading_action_, SIGNAL(preloadingError(const std::string&)),
            this, SLOT(slot_handlePreloadingError(const std::string&)));
    connect(preloading_action_, SIGNAL(applicationClosed(int)),
            this, SLOT(slot_handleApplicationClosed(int)));
}

void ProcessControl::setGUIReference(HMITesterControl* h)
{
    gui_reference_ = h;
}

HMITesterControl* ProcessControl::setGUIReference() const
{
    return gui_reference_;
}



/// ///
///
/// control to GUI
///
/// ///

/// ///
///
/// control from GUI
///
/// ///

///
/// play clicked
///
void ProcessControl::playQueuedTestCases()
{
    //if the execution is starting...
    //or it is being resumed..
    if (state_ == STOP || state_ == PAUSE_PLAY)
    {
        onPlay_playClicked();
    }
    //illegal action
    else
    {
        assert(0);//FIXME remove this assert
    }
}

void ProcessControl::onPlay_playClicked()
{
    DEBUG(D_PLAYBACK,"(ProcessControl::onPlay_playClicked)");
    //if the execution is starting...
    if (state_ == STOP)
    {
        // in no pending test cases to execute...
        if (_testcases_queue.size() < 1){
            DEBUG(D_ERROR,"(ProcessControl::onPlay_playClicked) No test cases to play.");
            return;
        }

        // get the first element of the queue
        _current_testcase = _testcases_queue.front();
        _testcases_queue.pop_front();

        //if the current test case is OK...
        if (_current_testcase)
        {
            assert(_current_testsuite->appId().empty() == false);
            assert(current_libPreload_path_.empty() == false);

            // restart communications
            _comm->resetAndStart();

            //launch the application
            bool ok = preloading_action_->launchApplication(
                        _current_testsuite->appId(),//app
                        current_libPreload_path_,//preload lib
                        STANDARD_OUTPUT_LOG_FILE,//output file
                        ERROR_OUTPUT_LOG_FILE);//error file

            //if not launched properly...
            if (!ok)
            {
                DEBUG(D_ERROR,"(ProcessControl::onPlay_playClicked) Error while launching the application.");
                return;
            }

            //start playback process
            DEBUG(D_PLAYBACK,"(ProcessControl::onPlay_playClicked) Starting playback process.");
            ok = playback_control_->runTestCase(_current_testcase, context_.speed);
            if (!ok)
            {
                //stop the app
                preloading_action_->stopApplication();

                DEBUG(D_ERROR,"(ProcessControl::onPlay_playClicked) Error while starting test case execution.");
                return;
            }

            //if everithing ok
            //update the state
            _setState(PLAY);

            DEBUG(D_PLAYBACK,"(ProcessControl::onPlay_playClicked) Playback process started. TestCase: " <<
                  _current_testcase->name());
            return;

        }
        //if not, invalid action
        else
        {
            DEBUG(D_ERROR,"(ProcessControl::onPlay_playClicked) Current test case is not valid.");
        }
    }
    //or it is being resumed..
    else if (state_ == PAUSE_PLAY)
    {
        playback_control_->resumeExecution();
        //update the state
        _setState(PLAY);
        return;
    }
    //illegal action
    else
    {
        assert(0);//FIXME remove this assert
    }
}

void ProcessControl::onRecord_playClicked()
{
    DEBUG(D_RECORDING,"(ProcessControl::onRecord_playClicked)");
    assert(0);//FIXME remove this assert
}

///
/// pause clicked
///
void ProcessControl::pauseClicked()
{
    //if it is playing...
    if (state_ == PLAY)
    {
        onPlay_pauseClicked();
    }
    //if it is recording...
    else if (state_ == RECORD)
    {
        onRecord_pauseClicked();
    }
    //illegal action
    else
    {
        assert(0);//FIXME remove this assert
    }
}

void ProcessControl::onPlay_pauseClicked()
{
    DEBUG(D_PLAYBACK,"(ProcessControl::onPlay_pauseClicked)");
    playback_control_->pauseExecution();

    //update the state
    _setState(PAUSE_PLAY);
}

void ProcessControl::onRecord_pauseClicked()
{
    DEBUG(D_RECORDING,"(ProcessControl::onRecord_pauseClicked)");
    recording_control_->pauseRecording();

    //update the state
    _setState(PAUSE_RECORD);
}

///
/// stop clicked
///
void ProcessControl::stopClicked()
{
    //if it is playing (or in pause)...
    if (state_ == PLAY || state_ == PAUSE_PLAY)
    {
        onPlay_stopClicked();
    }
    //if it is recording (or in pause)...
    else if (state_ == RECORD || state_ == PAUSE_RECORD)
    {
        onRecord_stopClicked();
    }
    //illegal action
    else
    {
        assert(0);//FIXME remove this assert
    }

    // set state
    //_setState(STOP); // not here. It will be set internally
}

void ProcessControl::onPlay_stopClicked()
{
    DEBUG(D_PLAYBACK,"(ProcessControl::onPlay_stopClicked)");
    playback_control_->stopExecution();

    // application is already killed above
}

void ProcessControl::onRecord_stopClicked()
{
    DEBUG(D_RECORDING,"(ProcessControl::onRecord_stopClicked)");
    recording_control_->stopRecording();

    //kill the application if enabled
    if (context_.keepAlive == false)
    {
        preloading_action_->stopApplication();
        DEBUG(D_BOTH,"(ProcessControl::onRecord_stopClicked) Keep alive disabled. Application stoped.");
    }
}

///
/// rec clicked
///
void ProcessControl::recClicked()
{
    //if the recording is beginning...
    //or it is being resumed..
    if (state_ == STOP || state_ == PAUSE_RECORD)
    {
        onRecord_recClicked();
    }
    //illegal action
    else
    {
        assert(0);//FIXME remove this assert
    }
}

void ProcessControl::onPlay_recClicked()
{
    DEBUG(D_PLAYBACK,"(ProcessControl::onPlay_recClicked)");
    assert(0);//FIXME remove this assert
}

void ProcessControl::onRecord_recClicked()
{
    DEBUG(D_RECORDING,"(ProcessControl::onRecord_recClicked) Start.");
    //if the recording is beginning...
    if (state_ == STOP)
    {
        //if the current test case has been set...
        if (_current_testcase)
        {
            // restart communications
            _comm->resetAndStart();

            //launch the application
            bool ok = preloading_action_->launchApplication(
                        _current_testsuite->appId(),//app
                        current_libPreload_path_,//preload lib
                        STANDARD_OUTPUT_LOG_FILE,//output file
                        ERROR_OUTPUT_LOG_FILE);//error file

            //if not is launched properly...
            if (!ok)
            {
                DEBUG(D_ERROR,"(ProcessControl::onRecord_recClicked) Error while launching the application.");
                return;
            }

            DEBUG(D_RECORDING,"(ProcessControl::onRecord_recClicked) App launched.");

            //update the state
            _setState(RECORD);

            //start recording process
            recording_control_->recordTestCase(_current_testcase);

            DEBUG(D_RECORDING,"(ProcessControl::onRecord_recClicked) Recording process started.");
        }
        //if not, invalid action
        else
        {
            DEBUG(D_ERROR,"(ProcessControl::onRecord_recClicked) Error while starting TestCase recording->");
            assert(0);//FIXME remove this assert
        }
    }
    //or if it is being resumed..
    else if (state_ == PAUSE_RECORD)
    {
        recording_control_->resumeRecording();
        //update the state
        _setState(RECORD);
    }
    //illegal action
    else
    {
        assert(0);//FIXME remove this assert
    }
}


/// /
/// test suite
/// /


DataModel::TestSuite* ProcessControl::loadTestSuiteObject(const std::string& file)
{
    try{
        return dataModel_manager_->getCurrentDataModelAdapter()->file2testSuite(file);
    }
    //if a conversion error occurs...
    catch (DataModelAdapter::conversion_error_exception&){
        DEBUG(D_ERROR,"(ProcessControl::loadTestSuiteObject) Error converting from a file.");
        return NULL;
    }
}

bool ProcessControl::openTestSuite(const std::string& file)
{
    DEBUG(D_BOTH,"(ProcessControl::openTestSuite)");

    ///get the testSuite object from the file
    DataModel::TestSuite* ts = loadTestSuiteObject(file);
    if (!ts)
        return false;


    //check if the binary exists    //FIXME remove Qt from here
    bool ok = QtUtils::isExecutableCP(QString(ts->appId().c_str()));
    if (!ok)
    {
        DEBUG(D_ERROR,"(ProcessControl::openTestSuite) Error: The file specified is not a valid or existing binary.");
        return false;
    }

    //save the current fileName
    current_filename_ = file;

    //if everithing OK...

    // FIXME: check if the memory is properly managed here
    _current_testsuite = ts;

    //update the internal state
    _setState(STOP);
    //update GUI
    assert(gui_reference_);
    gui_reference_->updateTestSuiteInfo(_current_testsuite);

    return true;
}

bool ProcessControl::newTestSuite(const std::string& file,
                                  const std::string& name,
                                  const std::string& appId)
{
    DEBUG(D_BOTH,"(ProcessControl::newTestSuite)");
    //create a new TestSuite
    DataModel::TestSuite* aux = _current_testsuite;
    _current_testsuite = new DataModel::TestSuite();

    //set the values to the test suite
    _current_testsuite->name(name);
    _current_testsuite->appId(appId);

    //dump the testSuite to a file
    if (!saveTestSuite(_current_testsuite,file))
    {
        DEBUG(D_ERROR, "(ProcessControl::newTestSuite) Error saving the testsuite file.");
        return false;
    }
    DEBUG(D_BOTH, "(ProcessControl::newTestSuite) TestSuite file updated.");

    //save the current fileName
    current_filename_ = file;

    // TODO: try/catch. if exception current = aux

    //if everything ok...

    // FIXME: Who deletes currentTestSuite_????

    //update the internal state
    _setState(STOP);

    //update GUI
    assert(gui_reference_);
    gui_reference_->updateTestSuiteInfo(_current_testsuite);
    return true;
}


bool ProcessControl::saveTestSuite(DataModel::TestSuite* ts,
                                   const std::string& file)
{
    try{
        dataModel_manager_->getCurrentDataModelAdapter()->testSuite2file(*ts, file);
        return true;
    }
    catch(DataModelAdapter::conversion_error_exception&){
        return false;
    }
}


///test case
bool ProcessControl::checkAndQueueTestCase(const std::string& tcName)
{
    DEBUG(D_PLAYBACK,"(ProcessControl::playTestCase)");
    //if the testSuite is valid...
    if (_current_testsuite)
    {
        DataModel::TestCase* tc = NULL;
        try {
            //and the test case exists...
            tc = _current_testsuite->getTestCase(tcName);
            // add to the queue
            _testcases_queue.push_back(tc);
            return true;
        }
        catch (DataModel::not_found&) {
            return false;
        }
    }

    return false;
}


bool ProcessControl::deleteTestCase(const std::string& tcName)
{
    DEBUG(D_BOTH,"(ProcessControl::deleteTestCase)");

    //if the testSuite is valid...
    if (_current_testsuite)
    {
        _current_testcase = NULL;
        try {
            _current_testsuite->deleteTestCase (tcName);

            //update the file
            if (!saveTestSuite(_current_testsuite, current_filename_))
            {
                DEBUG(D_ERROR, "(ProcessControl::deleteTestCase) Error saving the testsuite file.");
                return false;
            }

            //update the GUI
            gui_reference_->updateTestSuiteInfo(_current_testsuite);

            return true;
        } catch (DataModel::not_found&) {
            DEBUG(D_ERROR, "(ProcessControl::deleteTestCase) The Test Case does not exists.");
            return false;
        }
    }
    return false;
}

bool ProcessControl::recordTestCase(const std::string& tcName)
{
    DEBUG(D_RECORDING,"(ProcessControl::recordTestCase)");
    //if the testSuite is valid...
    if (_current_testsuite)
    {
        //and the test case exists...
        if (_current_testsuite->existsTestCase(tcName))
        {
            //return false because the testCase exists
            DEBUG(D_ERROR,"(ProcessControl::recordTestCase) The TestCase already exists.");
            return false;
        }
        //if it does not exist
        else
        {
            //TODO: may the currentTestCase_ has been created,
            //      not assigned to any TestSuite and not
            //      deleted? memory leak?
            //FIXME

            //create a new testCase to be recorded
            _current_testcase = new DataModel::TestCase();
            _current_testcase->name(tcName);
            DEBUG(D_RECORDING,"(ProcessControl::recordTestCase) Created new TestCase: " <<
                  tcName);
            return true;
        }
    }
    return false;
}

bool ProcessControl::recordExistingTestCase(const std::string& tcName)
{
    DEBUG(D_RECORDING,"(ProcessControl::recordExistingTestCase)");

    //if the testSuite is valid...
    if (_current_testsuite)
    {
        try
        {
            _current_testsuite->deleteTestCase (tcName);

            //set the test case as the current
            //create a new testCase to be recorded
            _current_testcase = new DataModel::TestCase();
            _current_testcase->name(tcName);
            DEBUG(D_RECORDING,"(ProcessControl::recordExistingTestCase) Redefining TestCase: " <<
                  tcName);

            return true;
        } catch (DataModel::not_found&)
        {
            return false;
        }
    }
    return false;
}


///
///process state
///
ProcessControl::OHTProcessState
ProcessControl::state() const
{
    return state_;
}

///
///process context
///
ProcessControl::OHTProcessContext &ProcessControl::context()
{
    return context_;
}

///
/// comm
///
void ProcessControl::slot_handleCommError(const std::string& s)
{
    DEBUG(D_ERROR, "(ProcessControl::slot_handleCommError) " << s);
}

/// ///
///
/// ExecutionObserver implementation
///
/// ///

void ProcessControl::executionThreadTerminated(int i)
{
    /// This method is called when the execution thread is finished.
    /// Note that that the AUT may be STILL RUNNING, so we can decide
    /// if it is closed or not.

    DEBUG(D_PLAYBACK,"(ProcessControl::executionThreadTerminated) Code = " << i);

    // not kill the application if enabled
    if (context_.keepAlive == false)
    {
        preloading_action_->stopApplication();
        DEBUG(D_BOTH,"(ProcessControl::stopClicked) Keep alive disabled. Application stoped.");
    }
}

void ProcessControl::completedPercentageNotification(int i)
{
    DEBUG(D_PLAYBACK,"(ProcessControl::completedPercentageNotification)");
    gui_reference_->setForm_playbackStatus(i);
}

/// ///
///
/// RecordingObserver implementation
///
/// ///

void ProcessControl::testRecordingFinished(DataModel::TestCase* tc)
{
    DEBUG(D_RECORDING,"(ProcessControl::testRecordingFinished)");

    //add the test case to the test suite
    _current_testsuite->addTestCase (_current_testcase);

    //update the GUI
    gui_reference_->updateTestSuiteInfo(_current_testsuite);

    //dump the testSuite to a file
    if (!saveTestSuite(_current_testsuite, current_filename_))
    {
        DEBUG(D_ERROR, "(ProcessControl::testRecordingFinished) Error saving the testsuite file.");
        return;
    }
    DEBUG(D_BOTH, "(ProcessControl::testRecordingFinished) TestSuite file updated.");
}

void ProcessControl::testItemsReceivedCounter(int i)
{
    DEBUG(D_RECORDING,"(ProcessControl::testItemsReceivedCounter)");
    gui_reference_->setForm_recordingStatus(i);
}

///
/// handled signals from preloading control
///

void ProcessControl::slot_handleApplicationClosed(int i)
{
    /// This method is called when the AUT is closed.
    /// During recording, application may be closed:
    ///  - manually -> stop was pressed
    ///  - automatically -> app was closed
    /// Note that during playback, application may be closed with:
    ///  - execution thread alive -> it was because an error
    ///  - execution thread finished -> normal execution

    DEBUG(D_BOTH, "(ProcessControl::slot_handleApplicationClosed) Code = " << i);

    //if playing... notify
    if (state_ == PLAY)
    {
        DEBUG(D_BOTH, "(ProcessControl::slot_handleApplicationClosed) Stop playback.");
        playback_control_->applicationFinished(); // calls executionThreadTerminated
        //update the state
        _setState(STOP);

        // stop communications
        _comm->stop();

        DEBUG(D_BOTH, "(ProcessControl::slot_handleApplicationClosed) Check for pending testcases.");
        // if queued test cases, go with them
        if (_testcases_queue.size() > 0)
            playQueuedTestCases();
    }
    //if recording... notify
    else if (state_ == RECORD)
    {
        DEBUG(D_BOTH, "(ProcessControl::slot_handleApplicationClosed) Stop recording.");
        recording_control_->applicationFinished();
        // set stop state
        _setState(STOP);

        // stop communications
        _comm->stop();
    }
}

void ProcessControl::slot_handlePreloadingError(const std::string& s)
{
    DEBUG(D_ERROR, "(ProcessControl::preloading_handleErrorNotification) " + s);
}

/// ///
///
/// control signaling handle
///
/// ///

void ProcessControl::handleControlSignaling ( DataModel::TestItem* ti)
{
    DEBUG(D_BOTH,"(ProcessControl::handleControlSignaling)");

    //if this event has a signaling type...
    if (ti->type() == Control::CTI_TYPE)
    {
        /// 0 -> control signaling OHT > PM
        //CTI_ERROR = 0;
        if (ti->subtype() == Control::CTI_ERROR)
        {
            DEBUG(D_BOTH,"(ProcessControl::handleControlSignaling) ERROR.");
            Control::CTI_Error *cti = static_cast<Control::CTI_Error*>(ti);
            handle_CTI_Error(cti->description());
        }
        // 90 -> control signaling PM > OHT
        //CTI_EVENT_EXECUTED = 91;
        else if (ti->subtype() == Control::CTI_EVENT_EXECUTED)
        {
            DEBUG(D_BOTH,"(ProcessControl::handleControlSignaling) Event Executed.");
            handle_CTI_EventExecuted();
        }
    }
}

void ProcessControl::handle_CTI_Error(const std::string& message)
{
    DEBUG(D_BOTH,"(ProcessControl::handle_CTI_Error)");
    DEBUG(D_BOTH,"(ProcessControl::handle_CTI_Error) Received error message from Preload Module"
          << message);
    //TODO
}

void ProcessControl::handle_CTI_EventExecuted()
{
    DEBUG(D_PLAYBACK,"(ProcessControl::handle_CTI_EventExecuted)");
    playback_control_->handleEventExecutedOnPreloadModule();
}

///
///support methods
///
void ProcessControl::_setState(OHTProcessState s)
{
    //STOP
    if (s == STOP && state_ != STOP)
    {
        //internal
        state_ = s;
        //GUI update
        gui_reference_->setForm_stopState();
    }
    //RECORD
    else if (s == RECORD && state_ != RECORD)
    {
        //internal
        state_ = s;
        //GUI update
        gui_reference_->setForm_recState();
    }
    //PLAY
    else if (s == PLAY && state_ != PLAY)
    {
        //internal
        state_ = s;
        //GUI update
        gui_reference_->setForm_playState();
    }
    //PAUSE_PLAY
    else if (s == PAUSE_PLAY && state_ != PAUSE_PLAY)
    {
        //internal
        state_ = s;
        //GUI update
        gui_reference_->setForm_pausePlayState();
    }
    //PAUSE_RECORD
    else if (s == PAUSE_RECORD && state_ != PAUSE_RECORD)
    {
        //internal
        state_ = s;
        //GUI update
        gui_reference_->setForm_pauseRecState();
    }
}




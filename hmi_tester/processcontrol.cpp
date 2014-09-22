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
#include "xmldatamodeladapter.h"
#include "controlsignaling.h"
#include "ohtconfig.h"
#include "linuxpreloadingaction.h"
#include "debug.h"
#include "qtutils.h"
#include "hmitestercontrol.h"

ProcessControl::ProcessControl()
{
    //variable initialization
    GUIReference_ = NULL;

    //state
    setState(INIT);

    //
    currentFilename_ = "";
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
    ///state
    setState(INIT);

    ///communication manager creation
    comm_.reset (new Comm(TCP_PORT, true));

    ///dataModelManager
    dataModelManager_ = new DataModelManager();
    if (DA_XML_ENABLED)
    {
        DataModelAdapter* adapter = new XMLDataModelAdapter();
        dataModelManager_->addDataModelAdapter(DA_XML, adapter);
        assert(dataModelManager_->setCurrentDataModelAdapter(DA_XML));
    }
    //TODO: add here new dataModel adapters!!!

    ///preloading action object
    if (PA_LINUX_ENABLED)
    {
        preloadingAction_ = new LinuxPreloadingAction();
    }
    //TODO: add here new preloading actions!!!
    assert(preloadingAction_);

    ///playback control creation
    playbackControl_.reset (new PlaybackControl(comm_.get(), this));

    ///recording control creation
    recordingControl_.reset(new RecordingControl(comm_.get(), this));

    ///
    /// signals connection
    ///

    //signals between this and Comm
    connect(comm_.get(),SIGNAL(error(const std::string&)),
            this,SLOT(handleCommError(const std::string&)));
    connect(comm_.get(),SIGNAL(receivedTestItem (DataModel::TestItem*)),
            this,SLOT(handleControlSignaling (DataModel::TestItem*)));

    //signals between preloadingAction and this
    connect(preloadingAction_, SIGNAL(preloadingError(const std::string&)),
            this, SLOT(preloading_handleErrorNotification(const std::string&)));
    connect(preloadingAction_, SIGNAL(applicationFinished(int)),
            this, SLOT(preloading_handleLaunchedApplicationFinished(int)));
}

void ProcessControl::GUIReference(HMITesterControl* h)
{
    GUIReference_ = h;
}

HMITesterControl* ProcessControl::GUIReference() const
{
    return GUIReference_;
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
void ProcessControl::playClicked()
{
    //if the execution is beginning...
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
    //if the execution is beginning...
    if (state_ == STOP)
    {
        //if the current test case has been set...
        if (currentTestCase_)
        {
            //launch the application
            bool ok = preloadingAction_->launchApplication(
                    currentTestSuite_->appId(),//app
                    LIB_PRELOAD,//preload lib
                    STANDARD_OUTPUT_FILE,//output file
                    ERROR_OUTPUT_FILE);//error file

            //if not is launched properly...
            if (!ok)
            {
                DEBUG(D_ERROR,"(ProcessControl::onPlay_playClicked) Error while launching the application.");
                return;
            }

            DEBUG(D_PLAYBACK,"(ProcessControl::onPlay_playClicked) Starting playback process.");
            //start playback process
            ok = playbackControl_->runTestCase(currentTestCase_);
            if (!ok)
            {
                //stop the app
                preloadingAction_->stopApplication();

                DEBUG(D_ERROR,"(ProcessControl::onPlay_playClicked) Error while starting test case execution.");
                return;
            }

            //if everithing ok
            //update the state
            setState(PLAY);

            DEBUG(D_PLAYBACK,"(ProcessControl::onPlay_playClicked) Playback process started. TestCase: " <<
                  currentTestCase_->name());
            return;

        }
        //if not, invalid action
        else
        {
            DEBUG(D_ERROR,"(ProcessControl::onPlay_playClicked) Error while starting TestCase playback.");
            assert(0);//FIXME remove this assert
        }
    }
    //or it is being resumed..
    else if (state_ == PAUSE_PLAY)
    {
        playbackControl_->resumeExecution();
        //update the state
        setState(PLAY);
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
    playbackControl_->pauseExecution();

    //update the state
    setState(PAUSE_PLAY);
}

void ProcessControl::onRecord_pauseClicked()
{
    DEBUG(D_RECORDING,"(ProcessControl::onRecord_pauseClicked)");
    recordingControl_->pauseRecording();

    //update the state
    setState(PAUSE_RECORD);
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

    //common behavior
    setState(STOP);

    //kill the application if enabled
    if (context_.keepAlive == false)
    {
        preloadingAction_->stopApplication();
        DEBUG(D_BOTH,"(ProcessControl::stopClicked) Keep alive disabled. Application stoped.");
    }
}

void ProcessControl::onPlay_stopClicked()
{
    DEBUG(D_PLAYBACK,"(ProcessControl::onPlay_stopClicked)");
    playbackControl_->stopExecution();
}

void ProcessControl::onRecord_stopClicked()
{
    DEBUG(D_RECORDING,"(ProcessControl::onRecord_stopClicked)");
    recordingControl_->stopRecording();
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
        if (currentTestCase_)
        {
            //launch the application
            bool ok = preloadingAction_->launchApplication(
                    currentTestSuite_->appId(),//app
                    LIB_PRELOAD,//preload lib
                    STANDARD_OUTPUT_FILE,//output file
                    ERROR_OUTPUT_FILE);//error file

            //if not is launched properly...
            if (!ok)
            {
                DEBUG(D_ERROR,"(ProcessControl::onRecord_recClicked) Error while launching the application.");
                return;
            }

            DEBUG(D_RECORDING,"(ProcessControl::onRecord_recClicked) App launched.");

            //update the state
            setState(RECORD);

            //start recording process
            recordingControl_->recordTestCase(currentTestCase_);

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
        recordingControl_->resumeRecording();
        //update the state
        setState(RECORD);
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
bool ProcessControl::openTestSuite(const std::string& file)
{
    DEBUG(D_BOTH,"(ProcessControl::openTestSuite)");

    ///get the testSuite object from the file
    DataModel::TestSuite* ts;
    try{
        ts = dataModelManager_->getCurrentDataModelAdapter()->file2testSuite(file);
    }
    //if a conversion error occurs...
    catch (DataModelAdapter::conversion_error&){
        DEBUG(D_ERROR,"(ProcessControl::openTestSuite) Error converting from a file.");
        return false;
    }

    //check if the binary exists
    bool ok = QtUtils::isExecutable(QString(ts->appId().c_str()));//FIXME remove Qt from here
    if (!ok)
    {
        DEBUG(D_ERROR,"(ProcessControl::openTestSuite) Error: The file specified is not a valid or existing binary.");
        return false;
    }

    //save the current fileName
    currentFilename_ = file;

    //if everithing OK...

    // FIXME: check if the memory is properly managed
    currentTestSuite_ = ts;

    //update the internal state
    setState(STOP);
    //update GUI
    assert(GUIReference_);
    GUIReference_->updateTestSuiteInfo(currentTestSuite_);

    return true;
}

bool ProcessControl::newTestSuite(const std::string& file,
                                  const std::string& name,
                                  const std::string& appId)
{
    DEBUG(D_BOTH,"(ProcessControl::newTestSuite)");
    //create a new TestSuite
    DataModel::TestSuite* aux = currentTestSuite_;
    currentTestSuite_ = new DataModel::TestSuite();

    //set the values to the test suite
    currentTestSuite_->name(name);
    currentTestSuite_->appId(appId);

    //dump the testSuite to a file
    dataModelManager_->getCurrentDataModelAdapter()->testSuite2file(*currentTestSuite_,file);
    DEBUG(D_BOTH, "(ProcessControl::newTestSuite) TestSuite file updated.");

    //save the current fileName
    currentFilename_ = file;

    // TODO: try/catch. if exception current = aux

    //if everything ok...

    // FIXME: Who deletes currentTestSuite_????

    //update the internal state
    setState(STOP);

    //update GUI
    assert(GUIReference_);
    GUIReference_->updateTestSuiteInfo(currentTestSuite_);
    return true;
}


///test case
bool ProcessControl::playTestCase(const std::string& tcName)
{
    DEBUG(D_PLAYBACK,"(ProcessControl::playTestCase)");
    //if the testSuite is valid...
    if (currentTestSuite_)
    {
        currentTestCase_ = NULL;
        try {
            //and the test case exists...
            currentTestCase_ = currentTestSuite_->getTestCase(tcName);
            return true;
        } catch (DataModel::not_found&) {
            return false;
        }
    }

    return false;
}


bool ProcessControl::deleteTestCase(const std::string& tcName)
{
    DEBUG(D_BOTH,"(ProcessControl::deleteTestCase)");

    //if the testSuite is valid...
    if (currentTestSuite_)
    {
        currentTestCase_ = NULL;
        try {
            currentTestSuite_->deleteTestCase (tcName);

            //update the file
            dataModelManager_->getCurrentDataModelAdapter()->testSuite2file(*currentTestSuite_, currentFilename_);

            //update the GUI
            GUIReference_->updateTestSuiteInfo(currentTestSuite_);

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
    if (currentTestSuite_)
    {
        //and the test case exists...
        if (currentTestSuite_->existsTestCase(tcName))
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
            //      deleted?
            //FIXME

            //create a new testCase to be recorded
            currentTestCase_ = new DataModel::TestCase();
            currentTestCase_->name(tcName);
            DEBUG(D_RECORDING,"(ProcessControl::recordTestCase) Crated new TestCase: " <<
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
    if (currentTestSuite_)
    {
        try
        {
            currentTestSuite_->deleteTestCase (tcName);

            //set the test case as the current
            //create a new testCase to be recorded
            currentTestCase_ = new DataModel::TestCase();
            currentTestCase_->name(tcName);
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
ProcessControl::ProcessState
        ProcessControl::state() const
{
    return state_;
}

///
///process context
///
ProcessControl::ProcessContext ProcessControl::context() const
{
    return context_;
}
ProcessControl::ProcessContext& ProcessControl::setContext()
{
    return context_;
}

///
///execution speed
///
void
ProcessControl::setExecutionSpeed(int speed)
{
    playbackControl_->handleExecutionSpeedChanged(speed);
    context_.speed = speed;
}

///
/// comm
///
void
ProcessControl::handleCommError(const std::string& s)
{
    DEBUG(D_ERROR, "(ProcessControl::handleCommError) " << s);
}

/// ///
///
/// ExecutionObserver implementation
///
/// ///

void
ProcessControl::executionThreadTerminated(int i)
{
    DEBUG(D_PLAYBACK,"(ProcessControl::executionThreadTerminated) Code = " << i);

    //update the state
    setState(STOP);

    //kill the application if enabled
    if (context_.keepAlive == false)
    {
        preloadingAction_->stopApplication();
        DEBUG(D_BOTH,"(ProcessControl::stopClicked) Keep alive disabled. Application stoped.");
    }
}

void ProcessControl::completedPercentageNotification(int i)
{
    DEBUG(D_PLAYBACK,"(ProcessControl::completedPercentageNotification)");
    GUIReference_->setLcdValueChanged(i);
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
    currentTestSuite_->addTestCase (currentTestCase_);

    //update the GUI
    GUIReference_->updateTestSuiteInfo(currentTestSuite_);

    //dump the testSuite to a file
    dataModelManager_->getCurrentDataModelAdapter()->testSuite2file(*currentTestSuite_, currentFilename_);
    DEBUG(D_BOTH, "(ProcessControl::testRecordingFinished) TestSuite file updated.");
}

void ProcessControl::testItemsReceivedCounter(int i)
{
    DEBUG(D_RECORDING,"(ProcessControl::testItemsReceivedCounter)");
    GUIReference_->setLcdValueChanged(i);
}

///
/// handled signals from preloading control
///

void ProcessControl::preloading_handleLaunchedApplicationFinished(int i)
{
    DEBUG(D_BOTH, "(ProcessControl::preloading_handleLaunchedApplicationFinished) Code = " << i);

    //if playing... notify
    if (state_ == PLAY)
    {
        DEBUG(D_BOTH, "(ProcessControl::preloading_handleLaunchedApplicationFinished) Stop execution.");
        playbackControl_->applicationFinished();

        //it is not necessary to set the stop state
        //because executionThread emits a signal
        //when finished

	executionThreadTerminated(0);

    }
    //if recording... notify
    else if (state_ == RECORD)
    {
        DEBUG(D_BOTH, "(ProcessControl::preloading_handleLaunchedApplicationFinished) Stop recording.");
        recordingControl_->applicationFinished();
        //set stop state (necessary because recording process
        //does not advise like playback process when the
        //execution thread is finished)
        setState(STOP);
    }
}

void ProcessControl::preloading_handleErrorNotification(const std::string& s)
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
    playbackControl_->handleEventExecutedOnPreloadModule();
}

///
///support methods
///
void ProcessControl::setState(ProcessState s)
{
    //STOP
    if (s == STOP && state_ != STOP)
    {
        //internal
        state_ = s;
        //GUI update
        GUIReference_->setForm_stopState();
    }
    //RECORD
    else if (s == RECORD && state_ != RECORD)
    {
        //internal
        state_ = s;
        //GUI update
        GUIReference_->setForm_recState();
    }
    //PLAY
    else if (s == PLAY && state_ != PLAY)
    {
        //internal
        state_ = s;
        //GUI update
        GUIReference_->setForm_playState();
    }
    //PAUSE_PLAY
    else if (s == PAUSE_PLAY && state_ != PAUSE_PLAY)
    {
        //internal
        state_ = s;
        //GUI update
        GUIReference_->setForm_pausePlayState();
    }
    //PAUSE_RECORD
    else if (s == PAUSE_RECORD && state_ != PAUSE_RECORD)
    {
        //internal
        state_ = s;
        //GUI update
        GUIReference_->setForm_pauseRecState();
    }
}




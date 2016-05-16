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
#ifndef PROCESSCONTROL_H
#define PROCESSCONTROL_H

#include <datamodel.h>
#include <comm.h>
#include <playbackcontrol.h>
#include <recordingcontrol.h>
#include <datamodelmanager.h>
#include <preloadingaction.h>
#include <executionobserver.h>
#include <recordingobserver.h>

#include <QObject>
#include <memory>

// Fwd
class HMITesterControl;


class ProcessControl :
        public QObject, public PlaybackObserver, public RecordingObserver
{

    Q_OBJECT

    ///
    ///type definition
    ///
public:
    //process state
    typedef enum
    {
        INIT, STOP, RECORD, PLAY, PAUSE_PLAY, PAUSE_RECORD
    } OHTProcessState;

    //process context
    typedef struct
    {
        bool keepAlive;
        float speed;
        bool showTesterOnTop;
    } OHTProcessContext;

public:

    ProcessControl(PreloadingAction* pa, DataModelAdapter* dma);
    ~ProcessControl();

    void initialize();

    void setGUIReference(HMITesterControl*);
    HMITesterControl* setGUIReference() const;

public:

    ///
    /// ExecutionObserver implementation
    ///
    virtual void executionThreadTerminated(int);
    virtual void completedPercentageNotification(int);

    ///
    /// RecordingObserver implementation
    ///
    virtual void testRecordingFinished(DataModel::TestCase*);
    virtual void testItemsReceivedCounter(int);

public slots:

    ///
    /// control to GUI
    ///

    ///
    /// control from GUI
    ///

    ///processes
    void playQueuedTestCases();
    void pauseClicked();
    void stopClicked();
    void recClicked();

    ///test suite
    bool openTestSuite(const std::string&);
    bool newTestSuite(const std::string& file,
                      const std::string& name,
                      const std::string& binaryPath);

    ///test case
    bool checkAndQueueTestCase(const std::string&);
    bool deleteTestCase(const std::string&);
    bool recordTestCase(const std::string&);
    bool recordExistingTestCase(const std::string&);

    ///process state
    OHTProcessState state() const;

    ///process context
    OHTProcessContext& context();

    ///
    /// comm
    ///
    void slot_handleCommError(const std::string&);



private slots:

    ///
    /// control from GUI
    ///

    /// on play
    void onPlay_playClicked();
    void onPlay_pauseClicked();
    void onPlay_stopClicked();
    void onPlay_recClicked();

    /// on record
    void onRecord_playClicked();
    void onRecord_pauseClicked();
    void onRecord_stopClicked();
    void onRecord_recClicked();

    ///
    /// handled signals from preloading control
    ///

    void slot_handleApplicationClosed(int);
    void slot_handlePreloadingError(const std::string&);

    ///
    /// control signaling handle
    ///

    void handleControlSignaling (DataModel::TestItem*);
    void handle_CTI_Error(const std::string& message);
    void handle_CTI_EventExecuted();


private:

    ///
    ///supporting methods
    ///

    void _setState(OHTProcessState);

    ///
    ///variables
    ///

    //GUI
    HMITesterControl* gui_reference_;

    //process controllers
    std::auto_ptr<PlaybackControl> playback_control_;
    std::auto_ptr<RecordingControl> recording_control_;

    //communication manager
    std::auto_ptr<Comm> _comm;

    //dataModel manager
    DataModelManager *dataModel_manager_;
    DataModelAdapter *dataModel_adapter_;

    //preloading action
    PreloadingAction* preloading_action_;

    //current testSuite
    DataModel::TestSuite* _current_testsuite;
    //current testCases
    std::list<DataModel::TestCase*> _testcases_queue;
    DataModel::TestCase* _current_testcase;
    //current fileName
    std::string current_filename_;
    // current lib preload path
    std::string current_libPreload_path_;
    // current oht bin path
    std::string current_oht_path_;

    //process state
    OHTProcessState state_;

    //process context
    OHTProcessContext context_;
};

#endif // PROCESSCONTROL_H

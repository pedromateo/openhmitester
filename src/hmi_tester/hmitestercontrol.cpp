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

#include "hmitestercontrol.h"

#include <cassert>
#include <iostream>

#include <ohtbaseconfig.h>
#include <debug.h>
#include <newtsdialog.h>
#include <edittsdialog.h>
#include <newtcdialog.h>
#include <qtutils.h>

#include <QFile>
#include <QWindow>
#include <QDir>

HMITesterControl::HMITesterControl(PreloadingAction *pa, DataModelAdapter *dma, QWidget *parent)
    : QMainWindow(parent)
{
    // create the GUI
    ui.setupUi(this);

    ///
    /// create overall process controller
    _processControl = new ProcessControl(pa, dma);
    _processControl->setGUIReference(this);
    _processControl->initialize();
    //set config values
    _processControl->context().keepAlive = false;
    _processControl->context().showTesterOnTop = true;
    _processControl->context().speed = 1;

    ///
    /// initialize GUI
    _initializeForm();

    this->resize(220,62); // FIXME try to fix this in the designer
    this->_keepAlive = true;
}

HMITesterControl::~HMITesterControl()
{
}

/// ///
///
/////initialization
///
/// ///

void HMITesterControl::_initializeForm()
{

    ///
    /// menu
    ///

    _initializeMenu();

    ///
    /// connecting signals and slots
    ///

    //buttons
    connect(ui.tb_play,SIGNAL(clicked()),this,SLOT(tb_play_clicked()));
    connect(ui.tb_pause,SIGNAL(clicked()),this,SLOT(tb_pause_clicked()));
    connect(ui.tb_stop,SIGNAL(clicked()),this,SLOT(tb_stop_clicked()));
    connect(ui.tb_rec,SIGNAL(clicked()),this,SLOT(tb_rec_clicked()));

    //form visualization states
    connect(this, SIGNAL(form_initState()),
            this, SLOT(_form_initState()),
            Qt::QueuedConnection);
    connect(this, SIGNAL(form_stopState()),
            this, SLOT(_form_stopState()),
            Qt::QueuedConnection);
    connect(this, SIGNAL(form_playState()),
            this, SLOT(_form_playState()),
            Qt::QueuedConnection);
    connect(this, SIGNAL(form_recState()),
            this, SLOT(_form_recState()),
            Qt::QueuedConnection);
    connect(this, SIGNAL(form_pausePlayState()),
            this, SLOT(_form_pausePlayState()),
            Qt::QueuedConnection);
    connect(this, SIGNAL(form_pauseRecState()),
            this, SLOT(_form_pauseRecState()),
            Qt::QueuedConnection);
    connect(this, SIGNAL(form_playbackStatus(int)),
            this, SLOT(_form_playbackStatus(int)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(form_recordingStatus(int)),
            this, SLOT(_form_recordingStatus(int)),
            Qt::QueuedConnection);
    connect(_processControl, SIGNAL(endExecutionTest()),
            this, SLOT(_test_ended_handle()),
            Qt::QueuedConnection);


    ///
    /// aspect settings
    ///

    ///initialize visualization
    _form_initState();

    // disable minimize button
    //this->setWindowFlags(Qt::Dialog);

    //check always on top
    action_showTesterOnTop_triggered(true);
}

void HMITesterControl::_initializeMenu()
{
    ///
    /// connecting signals and slots
    ///


    connect(ui.action1x,SIGNAL(triggered(bool)),this,SLOT(action_speed1x_triggered()));
    connect(ui.action05x,SIGNAL(triggered(bool)),this,SLOT(action_speed05x_triggered()));
    connect(ui.action2x,SIGNAL(triggered(bool)),this,SLOT(action_speed2x_triggered()));
    connect(ui.action4x,SIGNAL(triggered(bool)),this,SLOT(action_speed4x_triggered()));
    connect(ui.actionKeepAlive,SIGNAL(triggered(bool)),this,SLOT(action_keepAlive_triggered(bool)));
    connect(ui.actionShowTesterOnTop,SIGNAL(triggered(bool)),this,SLOT(action_showTesterOnTop_triggered(bool)));
    connect(ui.action_Open,SIGNAL(triggered(bool)),this,SLOT(action_open_triggered()));
    connect(ui.action_Edit,SIGNAL(triggered(bool)),this,SLOT(action_edit_triggered()));
    connect(ui.action_New,SIGNAL(triggered(bool)),this,SLOT(action_new_triggered()));
    connect(ui.action_Close,SIGNAL(triggered(bool)),this,SLOT(action_close_triggered()));
    connect(ui.action_Exit,SIGNAL(triggered(bool)),this,SLOT(action_exit_triggered()));


    ///
    /// build menu
    ///

    //file
    _menu_Main = ui.menuBar->findChild<QMenu*> ( "menu_Main" );
    assert ( _menu_Main );
    _menu_File = _menu_Main->findChild<QMenu*> ( "menu_File" );
    assert ( _menu_File );
    //config
    _menu_Config = _menu_Main->findChild<QMenu*> ( "menu_Config" );
    assert ( _menu_Config );
    _menu_Speed = _menu_Config->findChild<QMenu*> ( "menu_Speed" );
    assert ( _menu_Speed );
    _speedActionGroup = new QActionGroup ( _menu_Speed );
    _speedActionGroup->addAction( ui.action1x );
    _speedActionGroup->addAction( ui.action05x );
    _speedActionGroup->addAction( ui.action2x );
    _speedActionGroup->addAction( ui.action4x );

    // add popup menu to menu toolbutton
    ui.tb_menu->setMenu(_menu_Main);
    ui.tb_menu->setPopupMode(QToolButton::InstantPopup);

    ///
    /// play test case menu
    _playlistMenu = NULL;
    _playlistMenu_agroup = NULL;

    /// set default values
    updateTestSuiteInfo(NULL);
}

/// ///
///
/////button handlers
///
/// ///


void HMITesterControl::tb_play_clicked()
{
    DEBUG(D_GUI,"(HMITesterControl::tb_play_clicked)");

    //if the state == STOP it has to provide the
    //test case to play
    if (_processControl->state() == ProcessControl::STOP)
    {
        // clear selection and show menu
        ui.tb_play->showMenu();

        /// once an option has been selected...


        //get the name of the selected testCase
        QAction *actionSelected = _playlistMenu_agroup->checkedAction();
        if ( !actionSelected )
        {
            QtUtils::newErrorDialog ( "There is not a selected Test Case." );
            return;
        }

        // get name of item have been selected by user
        QString nameActionSelected = actionSelected->text();

        // case: play all in a row
        if (nameActionSelected == "All in a row")
        {
            QString tcName;
            bool ok = false;
            const int total_tc = _playlistMenu_agroup->actions().length() - 1;
            // queue all test cases - avoid last QAction
            for (int i = 0; i < total_tc; i++)
            {
                tcName = _playlistMenu_agroup->actions().at(i)->text();
                ok = _processControl->checkAndQueueTestCase(tcName.toStdString());
                if (!ok)
                {
                    DEBUG(D_ERROR,"(HMITesterControl::tb_play_clicked) Row playback. Test case not found.");
                }
                else{
                    DEBUG(D_GUI,"(HMITesterControl::tb_play_clicked) Queuing testcase "
                          << i + 1 << "/" << total_tc);
                }
            }
            // play test cases
            _processControl->playQueuedTestCases();
        }

        // case: play a single test case
        else
        {
            bool ok = _processControl->checkAndQueueTestCase(nameActionSelected.toStdString());
            if (!ok)
            {
                QtUtils::newErrorDialog ( "The selected Test Case is not valid." );
                return;
            }

            //indicate that the play button has been pressed
            DEBUG(D_GUI,"(HMITesterControl::tb_play_clicked) Start Play or Resume process.");
            _processControl->playQueuedTestCases();
        }

        // deselect option
        if (actionSelected){
            actionSelected->setChecked(false);
        }
    }
}

void HMITesterControl::tb_pause_clicked()
{
    DEBUG(D_GUI,"(HMITesterControl::tb_pause_clicked)");
    _processControl->pauseClicked();
    _set_statusbar_text("Paused");
}

void HMITesterControl::tb_stop_clicked()
{
    DEBUG(D_GUI,"(HMITesterControl::tb_stop_clicked)");
    _processControl->stopClicked();
}

void HMITesterControl::tb_rec_clicked()
{
    DEBUG(D_GUI,"(HMITesterControl::tb_rec_clicked)");

    //if it is a new Test Case recording...
    if (_processControl->state() == ProcessControl::STOP)
    {
        //get the name of the new test case
        NewTCDialog newtcd;
        int result = newtcd.exec();

        // if everything ok...
        if (result){
            //call the controller method
            bool ok = _processControl->recordTestCase(newtcd.getTestcaseName().toStdString());

            //process checking
            //if the testCase already exists
            if (!ok)
            {
                //ask the user
                bool cont = QtUtils::showOkCancelDialog(
                            QString("TestCase '" + newtcd.getTestcaseName() + "' already exists. Do you want to redefine it?"));
                //if the testCase has to be redefined
                if (cont)
                {
                    ok = _processControl->recordExistingTestCase(newtcd.getTestcaseName().toStdString());
                    //if an error occurs...
                    if (!ok)
                    {
                        QtUtils::newErrorDialog("The new TestCase cannot be created. Wrong data.");
                        return;
                    }
                }
                //if not...
                else
                {
                    return;
                }
            }

            // everything ok? go on with recording
            DEBUG(D_GUI,"(HMITesterControl::tb_rec_clicked) Starting record process.");
            _processControl->recClicked();
        }
        // if rejected...
        else{
            // do nothing
        }
    }
}

void HMITesterControl::openFileAndSetupData(QString path)
{
    DEBUG(D_GUI,"(HMITesterControl::openFileAndSetupData)");

    if (path == "") return;

    //open the testSuite
    bool ok = _processControl->openTestSuite(path.toStdString());
    if (!ok)
    {
        QtUtils::newErrorDialog("The TestSuite cannot be loaded.");
        return;
    }

    // enable close menu option
    ui.action_Close->setEnabled(true);

    // do dirs
    _settings.setValue(SETT_LAST_OPEN_DIR, path);
    _settings.endGroup();

}



/// ///
///
/////menu actions handler
///
/// ///


void HMITesterControl::action_open_triggered()
{
    DEBUG(D_GUI,"(HMITesterControl::action_open_triggered)");

    _settings.beginGroup("HMITesterControl");
    QString lastOpenDir = _settings.value(SETT_LAST_OPEN_DIR, QDir::homePath()).toString();

    //ask for the TestSuite
    QString path = "";
    path = QtUtils::openFileDialog("Please, select the file that contains the TestSuite:",
                                   lastOpenDir,
                                   "*." OHT_FILE_EXTENSION);
    if (path == "") return;

    //open the testSuite
    bool ok = _processControl->openTestSuite(path.toStdString());
    if (!ok)
    {
        QtUtils::newErrorDialog("The TestSuite cannot be loaded.");
        return;
    }

    // enable close menu option
    ui.action_Close->setEnabled(true);

    // do dirs
    _settings.setValue(SETT_LAST_OPEN_DIR, path);
    _settings.endGroup();

    //reconfigure the GUI
    //form_stopState();
}


void HMITesterControl::action_edit_triggered()
{
    DEBUG(D_GUI,"(HMITesterControl::action_edit_triggered)");
    _settings.beginGroup("HMITesterControl");
    QString lastOpenDir = _settings.value(SETT_LAST_OPEN_DIR, QDir::homePath()).toString();

    //ask for the TestSuite
    QString path = "";
    path = QtUtils::openFileDialog("Please, select the file that contains the TestSuite:",
                                   lastOpenDir,
                                   "*." OHT_FILE_EXTENSION);
    if (path == "") return;

    //load the testSuite object
    DataModel::TestSuite* ts = _processControl->loadTestSuiteObject(path.toStdString());
    if (!ts)
    {
        QtUtils::newErrorDialog("The TestSuite object cannot be loaded.");
        return;
    }

    //load the testSuite editor dialog
    EditTSDialog edittsd(ts);
    int result = edittsd.exec();
    if (result){
        //call the controller method
        bool ok = _processControl->saveTestSuite(ts,path.toStdString());
        if (!ok)
        {
            QtUtils::newErrorDialog("The TestSuite cannot be saved. Wrong data.");
            return;
        }
    }
    // if rejected...
    else{
        // do nothing
    }

    _settings.setValue(SETT_LAST_OPEN_DIR, path);
    _settings.endGroup();

    //reconfigure the GUI
    //form_stopState();
}

void HMITesterControl::action_new_triggered()
{
    DEBUG(D_GUI,"(HMITesterControl::action_new_triggered)");

    // ask for new testsuite data
    NewTSDialog newtsd;
    int result = newtsd.exec();

    // if everything ok...
    if (result){
        //call the controller method
        bool ok = _processControl->newTestSuite(
                    newtsd.getTestsuitePath().toStdString(),
                    newtsd.getTestsuiteName().toStdString(),
                    newtsd.getAUTPath().toStdString(),
                    newtsd.getListArgument().toStdString());

        //process checking
        if (!ok)
        {
            QtUtils::newErrorDialog("The TestSuite cannot be created. Wrong data.");
            return;
        }
    }
    // if rejected...
    else{
        // do nothing
    }
}

void HMITesterControl::action_close_triggered()
{
    DEBUG(D_GUI,"(HMITesterControl::action_close_triggered)");

    //close the testSuite
    bool ok = _processControl->closeCurrentTestSuite();
    if (!ok)
    {
        QtUtils::newErrorDialog("The TestSuite cannot be closed.");
        return;
    }
}

void HMITesterControl::action_exit_triggered()
{
    DEBUG(D_GUI,"(HMITesterControl::action_exit_triggered)");
    exit(0);
}

void HMITesterControl::action_speed1x_triggered()
{
    DEBUG(D_GUI,"(HMITesterControl::action_speed1x_triggered)");
    _processControl->context().speed = 1;
}

void HMITesterControl::action_speed05x_triggered()
{
    DEBUG(D_GUI,"(HMITesterControl::action_speed05x_triggered)");
    _processControl->context().speed = 0.5;
}

void HMITesterControl::action_speed2x_triggered()
{
    DEBUG(D_GUI,"(HMITesterControl::action_speed2x_triggered)");
    _processControl->context().speed = 2;
}

void HMITesterControl::action_speed4x_triggered()
{
    DEBUG(D_GUI,"(HMITesterControl::action_speed4x_triggered)");
    _processControl->context().speed = 4;
}


void HMITesterControl::action_keepAlive_triggered(bool b)
{
    DEBUG(D_GUI,"(HMITesterControl::action_keepAlive_triggered)");
    _processControl->context().keepAlive = b;
}

void HMITesterControl::action_showTesterOnTop_triggered(bool b)
{
    DEBUG(D_GUI,"(HMITesterControl::action_showTesterOnTop_triggered)");
    _processControl->context().showTesterOnTop = b;

    QWidget* w = this;
    int flags = w->windowFlags();

    //action
    if (b)
    {
        flags |= Qt::WindowStaysOnTopHint;
    }
    else
    {
        flags ^= Qt::WindowStaysOnTopHint;

    }
    w->setParent ( dynamic_cast<QWidget*> ( w->parent() ),static_cast<Qt::WindowFlags> ( flags ) );
}


/// ///
///
/////global shortcut
///
/// ///

void HMITesterControl::action_global_shortcut_triggered()
{
    DEBUG(D_GUI,"(HMITesterControl::action_global_shortcut_triggered)");
    _d("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXxx");
}

/// ///
///
/////testSuite handling
///
/// ///


void HMITesterControl::_playTestCaseSelected_triggered(bool)
{
    DEBUG(D_GUI,"(HMITesterControl::PlayTestCaseSelected_triggered)");
    //nothing is performed here
    //tb_play_clicked();

}



void HMITesterControl::updateTestSuiteInfo(DataModel::TestSuite* ts)
{
    DEBUG(D_GUI,"(HMITesterControl::updateTestSuiteInfo)");
    //asserts
    assert(_menu_File);
    assert(_menu_Config);
    assert(_menu_Speed);
    assert(_speedActionGroup);

    /// If ts != NULL -> ts is valid and everything is set
    /// If ts == NULL -> no valid ts and everything is reset to default

    ///
    /// ts != NULL

    if (ts != NULL){

        //set tSuite name
        ui.actionTsuiteName->setText(QString(ts->name().c_str()));

        ///
        /// clean and fill playlist
        ///

        // create menu if needed
        if (_playlistMenu == NULL)
            _playlistMenu = new QMenu(ui.tb_play);
        if (_playlistMenu_agroup == NULL)
            _playlistMenu_agroup = new QActionGroup(_playlistMenu);

        // clear existing actions
        foreach ( QAction *a, _playlistMenu_agroup->actions())
        {
            _playlistMenu_agroup->removeAction(a);
            delete a;
        }

        //if testCase count > 0
        if ( ts->count() > 0 )
        {
            // add new actions for current test cases
            QAction *a = NULL;
            QMenu *m = NULL;
            DataModel::TestSuite::TestCaseList::const_iterator tcit;
            //for each test case at the list...
            for(tcit = ts->testCases().begin(); tcit != ts->testCases().end(); tcit++)
            {
                const DataModel::TestCase &tc = *tcit;
                QString qname(tc.name().c_str());

                a = _playlistMenu->addAction(qname);
                _playlistMenu_agroup->addAction(a);
                a->setCheckable(true);
                a->setChecked(false);
                //connect(a, SIGNAL(triggered(bool)),
                //        this, SLOT(_playTestCaseSelected_triggered(bool)));
            }

            // add extra action for "all in a row"
            _playlistMenu->addSeparator();
            a = _playlistMenu->addAction("All in a row");
            _playlistMenu_agroup->addAction(a);
            a->setCheckable(true);
            a->setChecked(false);

            // provide the tool button with the menu
            ui.tb_play->setMenu(_playlistMenu);
            // check first action by default
            //_playlistMenu_agroup->actions().first()->setChecked(true);

        }
        //else disable the playlist
        else
        {
            ui.tb_play->setMenu(NULL);
        }

        //update the menu
        ui.menuBar->update();

        // set status
        _set_statusbar_text(QString("Testsuite: ") + QString(ts->name().c_str()));
    }

    ///
    /// ts == NULL

    else {

        //set tSuite name
        ui.actionTsuiteName->setText("No testsuite loaded");


        ///
        /// disable play menu

        ui.tb_play->setMenu(NULL);


        ///
        /// clean _playlistMenu

        // create menu if needed
        if (_playlistMenu == NULL)
            _playlistMenu = new QMenu(ui.tb_play);
        if (_playlistMenu_agroup == NULL)
            _playlistMenu_agroup = new QActionGroup(_playlistMenu);

        // clear existing actions
        foreach ( QAction *a, _playlistMenu_agroup->actions())
        {
            _playlistMenu_agroup->removeAction(a);
            delete a;
        }

        //update the menu
        ui.menuBar->update();

        // set status
        _set_statusbar_text(QString("No testsuite loaded"));
    }
}



///
/// \brief automationRunTestSuite
/// \param path
///
void HMITesterControl::automationRunTestSuite(QString path)
{
    this->openFileAndSetupData(path);
    this->playAllTestCase();
    this->_keepAlive = false;

}

///
/// \brief HMITesterControl::playAllTestCase
///
void HMITesterControl::playAllTestCase()
{
    //if the state == STOP it has to provide the
    //test case to play
    if (_processControl->state() == ProcessControl::STOP)
    {
        // case: play all in a row
        QString tcName;
        bool ok = false;
        const int total_tc = _playlistMenu_agroup->actions().length() - 1;
        // queue all test cases - avoid last QAction
        for (int i = 0; i < total_tc; i++)
        {
            tcName = _playlistMenu_agroup->actions().at(i)->text();
            ok = _processControl->checkAndQueueTestCase(tcName.toStdString());
            if (!ok)
            {
                DEBUG(D_ERROR,"(HMITesterControl::tb_play_clicked) Row playback. Test case not found.");
            }
            else{
                DEBUG(D_GUI,"(HMITesterControl::tb_play_clicked) Queuing testcase "
                      << i + 1 << "/" << total_tc);
            }
        }
        // play test cases
        _processControl->playQueuedTestCases();
    }
}

///
/// \brief _test_ended_handle
///
void HMITesterControl::_test_ended_handle()
{
    if (_keepAlive == false)
    {
        qDebug() <<"HMITesterControl::_test_ended_handle(), Automation mode, caling exit ...."<< "\n";
        this->close();
    }
}


/// ///
///
/// form visualization
///
/// ///


void HMITesterControl::setForm_initState()
{
    emit form_initState();
}

void HMITesterControl::_form_initState()
{
    //menu
    ui.menu_File->setEnabled(true);
    ui.action_New->setEnabled(true);
    ui.action_Open->setEnabled(true);
    ui.action_Edit->setEnabled(true);
    ui.action_Close->setEnabled(false);
    ui.menuBar->setVisible(false);
    //buttons
    _setEnableAndVisible(ui.tb_menu,true);
    _setEnableAndVisible(ui.tb_play,false);
    _setEnableAndVisible(ui.tb_pause,false);
    _setEnableAndVisible(ui.tb_stop,false);
    _setEnableAndVisible(ui.tb_rec,false);
    ui.tb_play->setVisible(true);
    ui.tb_rec->setVisible(true);
    // window opacity
    setWindowOpacity(IDLE_OPACITY);
}

void HMITesterControl::setForm_stopState()
{
    emit form_stopState();
}

void HMITesterControl::_form_stopState()
{
    //menu
    _setEnableAndVisible(ui.tb_menu,true);
    ui.menu_File->setEnabled(true);
    ui.action_New->setEnabled(false);
    ui.action_Open->setEnabled(false);
    ui.action_Edit->setEnabled(false);
    ui.action_Close->setEnabled(true);
    ui.menu_Config->setEnabled(true);
    ui.menuBar->setVisible(false);
    //buttons
    _setEnableAndVisible(ui.tb_play,true);
    _setEnableAndVisible(ui.tb_pause,false);
    _setEnableAndVisible(ui.tb_stop,false);
    _setEnableAndVisible(ui.tb_rec,true);
    // window opacity
    setWindowOpacity(IDLE_OPACITY);
}


void HMITesterControl::setForm_playState()
{
    emit form_playState();
}

void HMITesterControl::_form_playState()
{
    //menu
    ui.menu_File->setEnabled(false);
    ui.menu_Config->setEnabled(false);
    ui.menuBar->setVisible(false);
    //buttons
    _setEnableAndVisible(ui.tb_menu,false);
    _setEnableAndVisible(ui.tb_play,false);
    _setEnableAndVisible(ui.tb_pause,false);//TODO
    _setEnableAndVisible(ui.tb_stop,true);
    _setEnableAndVisible(ui.tb_rec,false);
    ui.tb_pause->setVisible(true);
    // window opacity
    setWindowOpacity(RUNNING_OPACITY);
}


void HMITesterControl::setForm_recState()
{
    emit form_recState();
}

void HMITesterControl::_form_recState()
{
    //menu
    ui.menu_File->setEnabled(false);
    ui.menu_Config->setEnabled(false);
    ui.menuBar->setVisible(false);
    //buttons
    _setEnableAndVisible(ui.tb_menu,false);
    _setEnableAndVisible(ui.tb_play,false);
    _setEnableAndVisible(ui.tb_pause,false);//TODO
    _setEnableAndVisible(ui.tb_stop,true);
    _setEnableAndVisible(ui.tb_rec,false);
    ui.tb_pause->setVisible(true);
    // window opacity
    setWindowOpacity(RUNNING_OPACITY);
}


void HMITesterControl::setForm_pausePlayState()
{
    emit form_pausePlayState();
}

void HMITesterControl::_form_pausePlayState()
{
    //menu
    ui.menu_File->setEnabled(false);
    ui.menu_Config->setEnabled(false);// TODO allow this
    ui.menuBar->setVisible(false);
    //buttons
    _setEnableAndVisible(ui.tb_menu,true);
    _setEnableAndVisible(ui.tb_play,true);
    _setEnableAndVisible(ui.tb_pause,false);
    _setEnableAndVisible(ui.tb_stop,true);
    _setEnableAndVisible(ui.tb_rec,false);
    // window opacity
    setWindowOpacity(IDLE_OPACITY);
}


void HMITesterControl::setForm_pauseRecState()
{
    emit form_pauseRecState();
}

void HMITesterControl::_form_pauseRecState()
{
    //menu
    ui.menu_File->setEnabled(false);
    ui.menu_Config->setEnabled(true);
    ui.menuBar->setVisible(false);
    //buttons
    _setEnableAndVisible(ui.tb_menu,false);
    _setEnableAndVisible(ui.tb_play,false);
    _setEnableAndVisible(ui.tb_pause,false);
    _setEnableAndVisible(ui.tb_stop,true);
    _setEnableAndVisible(ui.tb_rec,true);
    // window opacity
    setWindowOpacity(IDLE_OPACITY);
}


void HMITesterControl::setForm_playbackStatus(int p)
{
    emit form_playbackStatus(p);
}

void HMITesterControl::_form_playbackStatus(int p)
{
    QString msg = "Playback " + QString::number(p) + "%";
    _set_statusbar_text(msg);
}

void HMITesterControl::setForm_recordingStatus(int p)
{
    emit form_recordingStatus(p);
}

void HMITesterControl::_form_recordingStatus(int p)
{
    QString msg = "Recorded " + QString::number(p) + " items.";
    _set_statusbar_text(msg);
}



// status bar messages
void HMITesterControl::_set_statusbar_text(const QString& msg, int timeout)
{
    ui.status->showMessage(msg,timeout);
}

void HMITesterControl::_set_statusbar_text(const char *msg, int timeout)
{
    _set_statusbar_text(QString(msg),timeout);
}




/// ///
///
/// supporting functions
///
/// ///

///
void HMITesterControl::_setEnableAndVisible(QWidget* target, bool b)
{
    target->setEnabled(b);
    //target->setVisible(b);
}


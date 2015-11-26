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

#include <debug.h>
#include <newtsdialog.h>
#include <newtcdialog.h>
#include <qtutils.h>
#include <ohtbaseconfig.h>

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
    connect(ui.action_New,SIGNAL(triggered(bool)),this,SLOT(action_new_triggered()));
    connect(ui.action_Exit,SIGNAL(triggered(bool)),this,SLOT(action_exit_triggered()));


    ///
    /// build menu
    ///

    //file
    mainMenu_ = ui.menuBar->findChild<QMenu*> ( "menu_Main" );
    assert ( mainMenu_ );
    fileMenu_ = mainMenu_->findChild<QMenu*> ( "menu_File" );
    assert ( fileMenu_ );
    //test suite
    tsuiteMenu_ = mainMenu_->findChild<QMenu*> ( "menu_TestSuite" );
    assert ( tsuiteMenu_ );
    playTcaseMenu_ = tsuiteMenu_->findChild<QMenu*> ( "menu_Play_Test_Case" );
    assert ( playTcaseMenu_ );
    deleteTcaseMenu_ = tsuiteMenu_->findChild<QMenu*> ( "menu_Delete_Test_Case" );
    assert ( deleteTcaseMenu_ );
    playTestCaseActionGroup_ = new QActionGroup ( playTcaseMenu_ );
    deleteTestCaseActionGroup_ = new QActionGroup ( deleteTcaseMenu_ );
    //config
    configMenu_ = mainMenu_->findChild<QMenu*> ( "menu_Config" );
    assert ( configMenu_ );
    speedMenu_ = configMenu_->findChild<QMenu*> ( "menu_Speed" );
    assert ( speedMenu_ );
    speedActionGroup_ = new QActionGroup ( speedMenu_ );
    speedActionGroup_->addAction( ui.action1x );
    speedActionGroup_->addAction( ui.action05x );
    speedActionGroup_->addAction( ui.action2x );
    speedActionGroup_->addAction( ui.action4x );

    // add popup menu to menu toolbutton
    ui.tb_menu->setMenu(mainMenu_);
    ui.tb_menu->setPopupMode(QToolButton::InstantPopup);

    ///
    /// play test case menu
    ///
    _playAndDeleteMenu = NULL;
    _playAndDeleteMenu_agroup = NULL;
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
        QAction *actionSelected = _playAndDeleteMenu_agroup->checkedAction();
        if ( !actionSelected )
        {
            QtUtils::newErrorDialog ( "There is not a selected Test Case." );
            return;
        }

        // case: play all in a row
        if (_playAndDeleteMenu_agroup->actions().last() == actionSelected)
        {
            QString tcName;
            bool ok = false;
            const int total_tc = _playAndDeleteMenu_agroup->actions().length() - 1;
            // queue all test cases - avoid last QAction
            for (int i = 0; i < total_tc; i++)
            {
                tcName = _playAndDeleteMenu_agroup->actions().at(i)->text();
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
            //set the test case to play
            QString tcName = actionSelected->text();
            bool ok = _processControl->checkAndQueueTestCase(tcName.toStdString());
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


/// ///
///
/////menu actions handler
///
/// ///


void HMITesterControl::action_open_triggered()
{
    DEBUG(D_GUI,"(HMITesterControl::action_open_triggered)");
    QString lastOpenDir = QDir::homePath();
    settings_.beginGroup("HMITesterControl");
    lastOpenDir = settings_.value("lastOpenDir", QDir::homePath()).toString();
    //ask for the TestSuite
    QString path = "";
    path = QtUtils::openFileDialog("Please, select the file that contains the TestSuite:",
                                   lastOpenDir,
                                   "*."OHT_FILE_EXTENSION);
    if (path == "") return;

    //open the testSuite
    bool ok = _processControl->openTestSuite(path.toStdString());
    if (!ok)
    {
        QtUtils::newErrorDialog("The TestSuite cannot be loaded.");
        return;
    }

    settings_.setValue("lastOpenDir", lastOpenDir);
    settings_.endGroup();

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
                    newtsd.getAUTPath().toStdString());

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
/////testSuite handling
///
/// ///


void HMITesterControl::_playTestCaseSelected_triggered(bool)
{
    DEBUG(D_GUI,"(HMITesterControl::PlayTestCaseSelected_triggered)");
    //nothing is performed here
    //tb_play_clicked();

}

void HMITesterControl::_deleteTestCaseSelected_triggered(bool)
{
    DEBUG(D_GUI,"(HMITesterControl::DeleteTestCaseSelected_triggered)");

    //get the name of the selected testCase
    QAction *actionSelected = deleteTestCaseActionGroup_->checkedAction();
    if ( !actionSelected )
    {
        QtUtils::newErrorDialog ( "There is not a selected Test Case to delete." );
        return;
    }
    QString tcName = actionSelected->text();

    //if it can be deleted
    if (_processControl->deleteTestCase(tcName.toStdString()))
    {
        QtUtils::newInfoDialog( "The Test Case " + tcName + " has been deleted." );
    }
    //if not
    else
    {
        QtUtils::newErrorDialog ( "The Test Case " + tcName + " cannot be deleted." );
    }
}

void HMITesterControl::updateTestSuiteInfo(DataModel::TestSuite* ts)
{
    DEBUG(D_GUI,"(HMITesterControl::updateTestSuiteInfo)");
    //asserts
    assert(fileMenu_);
    assert(tsuiteMenu_);
    assert(playTcaseMenu_);
    assert(deleteTcaseMenu_);
    assert(configMenu_);
    assert(speedMenu_);
    assert(playTestCaseActionGroup_);
    assert(deleteTestCaseActionGroup_);
    assert(speedActionGroup_);

    //activating tSuite menu and set name
    tsuiteMenu_->setEnabled(true);

    ui.actionTsuiteName->setText(QString(ts->name().c_str()));

    //clearing testCase lists
    QList<QAction*> actions = playTestCaseActionGroup_->actions();
    foreach ( QAction *a, actions )
    {
        playTestCaseActionGroup_->removeAction ( a );
        delete a;
    }
    actions = deleteTestCaseActionGroup_->actions();
    foreach ( QAction *a, actions )
    {
        deleteTestCaseActionGroup_->removeAction ( a );
        delete a;
    }

    //setting the test cases name in both
    //play-list and delete-list

    //if testCase count > 0
    if ( ts->count() > 0 )
    {
        /*QAction *action = NULL;

        DataModel::TestSuite::TestCaseList::const_iterator it;
        const DataModel::TestSuite::TestCaseList& tcList = ts->testCases();

        //for each test case at the list...
        for(it = tcList.begin(); it != tcList.end(); it++)
        {
            const DataModel::TestCase &tc = *it;
            QString qname(tc.name().c_str());
            //play menu
            action = playTcaseMenu_->addAction ( qname );
            action->setCheckable ( true );
            action->setChecked ( false );
            playTestCaseActionGroup_->addAction ( action );

            //delete menu
            action = deleteTcaseMenu_->addAction ( qname );
            action->setCheckable ( true );
            action->setChecked ( false );
            deleteTestCaseActionGroup_->addAction ( action );
            //connect action to the trigger handler
            connect(action, SIGNAL(toggled(bool)),
                    this, SLOT(_deleteTestCaseSelected_triggered(bool)));
        }

        //enabling menus
        tsuiteMenu_->setEnabled ( true );
        playTcaseMenu_->setEnabled ( true );
        deleteTcaseMenu_->setEnabled ( true );

        //selecting 1st testCase in play menu
        playTestCaseActionGroup_->actions().first()->setChecked(true);*/

        ///
        /// clean and fill _playAndDeleteMenu
        ///

        // create menu if needed
        if (_playAndDeleteMenu == NULL)
            _playAndDeleteMenu = new QMenu(ui.tb_play);
        if (_playAndDeleteMenu_agroup == NULL)
            _playAndDeleteMenu_agroup = new QActionGroup(_playAndDeleteMenu);

        // clear existing actions
        foreach ( QAction *a, _playAndDeleteMenu_agroup->actions())
        {
            _playAndDeleteMenu_agroup->removeAction(a);
            delete a;
        }

        // add new actions for current test cases
        QAction *a = NULL;
        QMenu *m = NULL;
        DataModel::TestSuite::TestCaseList::const_iterator tcit;
        //for each test case at the list...
        for(tcit = ts->testCases().begin(); tcit != ts->testCases().end(); tcit++)
        {
            const DataModel::TestCase &tc = *tcit;
            QString qname(tc.name().c_str());

            a = _playAndDeleteMenu->addAction(qname);
            _playAndDeleteMenu_agroup->addAction(a);
            a->setCheckable(true);
            a->setChecked(false);
            //connect(a, SIGNAL(triggered(bool)),
            //        this, SLOT(_playTestCaseSelected_triggered(bool)));
        }

        // add extra action for "all in a row"
        _playAndDeleteMenu->addSeparator();
        a = _playAndDeleteMenu->addAction("All in a row");
        _playAndDeleteMenu_agroup->addAction(a);
        a->setCheckable(true);
        a->setChecked(false);

        // provide the tool button with the menu
        ui.tb_play->setMenu(_playAndDeleteMenu);
        // check first action by default
        //_playAndDeleteMenu_agroup->actions().first()->setChecked(true);

    }
    //else disable the testSuite menu
    else
    {
        playTcaseMenu_->setEnabled ( false );
        deleteTcaseMenu_->setEnabled ( false );

        ///
        /// disable _playAndDeleteMenu
        ///

        ui.tb_play->setMenu(NULL);
    }

    //update the menu
    ui.menuBar->update();

    // set status
    _set_statusbar_text(QString("Testsuite: ") + QString(ts->name().c_str()));
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
    ui.menu_TestSuite->setEnabled(false);
    ui.menu_Config->setEnabled(false);
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
    ui.menu_TestSuite->setEnabled(true);
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
    ui.menu_TestSuite->setEnabled(false);
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
    ui.menu_TestSuite->setEnabled(false);
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
    ui.menu_TestSuite->setEnabled(false);
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
    ui.menu_TestSuite->setEnabled(false);
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


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
#include <generalinputdialog.h>
#include <qtutils.h>
#include <QFile>
#include <ohtbaseconfig.h>

HMITesterControl::HMITesterControl(PreloadingAction *pa, DataModelAdapter *dma, QWidget *parent)
    : QMainWindow(parent)
{
    // create process controller
    _processControl = new ProcessControl(pa, dma);

    // create and initialize GUI
    ui.setupUi(this);
    _initializeForm();
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
    /// connecting signals and slots
    ///

    //buttons
    connect(ui.tb_play,SIGNAL(clicked()),this,SLOT(tb_play_clicked()));
    connect(ui.tb_pause,SIGNAL(clicked()),this,SLOT(tb_pause_clicked()));
    connect(ui.tb_stop,SIGNAL(clicked()),this,SLOT(tb_stop_clicked()));
    connect(ui.tb_rec,SIGNAL(clicked()),this,SLOT(tb_rec_clicked()));

    //menu
    connect(ui.action100,SIGNAL(triggered(bool)),this,SLOT(action_speed100_triggered()));
    connect(ui.action75,SIGNAL(triggered(bool)),this,SLOT(action_speed75_triggered()));
    connect(ui.action50,SIGNAL(triggered(bool)),this,SLOT(action_speed50_triggered()));
    connect(ui.action25,SIGNAL(triggered(bool)),this,SLOT(action_speed25_triggered()));
    connect(ui.actionKeepAlive,SIGNAL(triggered(bool)),this,SLOT(action_keepAlive_triggered(bool)));
    connect(ui.actionShowTesterOnTop,SIGNAL(triggered(bool)),this,SLOT(action_showTesterOnTop_triggered(bool)));
    connect(ui.action_Open,SIGNAL(triggered(bool)),this,SLOT(action_open_triggered()));
    connect(ui.action_New,SIGNAL(triggered(bool)),this,SLOT(action_new_triggered()));
    connect(ui.action_Exit,SIGNAL(triggered(bool)),this,SLOT(action_exit_triggered()));

    //lcd counter
    connect(this, SIGNAL(changeLcdValue(int)),
            this, SLOT(_changeLcdValue(int)),
            Qt::QueuedConnection);

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

    ///
    /// menu settings
    ///

    ///getting menus
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
    speedActionGroup_->addAction ( ui.action100 );
    speedActionGroup_->addAction ( ui.action75 );
    speedActionGroup_->addAction ( ui.action50 );
    speedActionGroup_->addAction ( ui.action25 );

    ///
    /// overall process controller settings
    ///

    //create and initialize
    _processControl->GUIReference(this);
    _processControl->initialize();
    //set config values
    _processControl->setContext().keepAlive = false;
    _processControl->setContext().showTesterOnTop = true;
    _processControl->setExecutionSpeed(50);

    ///initialize visualization
    form_initState();

    ///
    /// aspect settings
    ///

    // disable minimize button
    //this->setWindowFlags(Qt::Dialog);

    //check always on top
    action_showTesterOnTop_triggered(true);

    // adjust hmi tester size
    //ui.centralWidget->adjustSize();
    //this->adjustSize();
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
        //get the name of the selected testCase
        QAction *actionSelected = playTestCaseActionGroup_->checkedAction();
        if ( !actionSelected )
        {
            QtUtils::newErrorDialog ( "There is not a selected Test Case." );
            return;
        }
        QString tcName = actionSelected->text();

        //set the test case to play
        bool ok = _processControl->playTestCase(tcName.toStdString());
        if (!ok)
        {
            QtUtils::newErrorDialog ( "The selected Test Case is not valid." );
            return;
        }
    }

    //indicate that the play button has been pressed
    DEBUG(D_GUI,"(HMITesterControl::tb_play_clicked) Start Play or Resume process.");
    _processControl->playClicked();
}

void HMITesterControl::tb_pause_clicked()
{
    DEBUG(D_GUI,"(HMITesterControl::tb_pause_clicked)");
    _processControl->pauseClicked();
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
        QString data = "";
        QStringList values;
        values << "Name of the new Testcase:";
        QStringList results = GeneralInputDialog::askForValues(values);
        if (results.size() > 0)//OK
        {
            data = results[0];
        }
        else//Cancel
        {
            return;
        }

        //give the name of the new tcase to the processcontrol
        bool ok = _processControl->recordTestCase(data.toStdString());

        //if the testCase already exists
        if (!ok)
        {
            //ask the user
            bool cont = QtUtils::showOkCancelDialog("The TestCase\""
                                                    + data
                                                    + "\" already exists. Do you want to redefine it?");
            //if the testCase has to be redefined
            if (cont)
            {
                ok = _processControl->recordExistingTestCase(data.toStdString());
                //if an error occurs...
                if (!ok)
                {
                    QtUtils::newErrorDialog("The new TestCase cannot be created.");
                    return;
                }
            }
            //if not...
            else
            {
                return;
            }
        }
    }

    //indicate the rec button has been pressed
    DEBUG(D_GUI,"(HMITesterControl::tb_rec_clicked) Starting record process.");
    _processControl->recClicked();
}


/// ///
///
/////menu actions handler
///
/// ///


void HMITesterControl::action_open_triggered()
{
    DEBUG(D_GUI,"(HMITesterControl::action_open_triggered)");

    //ask for the TestSuite
    QString path = "";
    path = QtUtils::openFileDialog("Please, select the file that contains the TestSuite:",
                                   ".",//TODO add dir reminder
                                   "*");
    if (path == "") return;

    //open the testSuite
    bool ok = _processControl->openTestSuite(path.toStdString());
    if (!ok)
    {
        QtUtils::newErrorDialog("The TestSuite cannot be loaded.");
        return;
    }

    //reconfigure the GUI
    //form_stopState();
}

void HMITesterControl::action_new_triggered()
{
    DEBUG(D_GUI,"(HMITesterControl::action_new_triggered)");

    //ask for the name of the TestSuite
    QString name;
    QStringList values;
    values << "Name of the TestSuite:";
    QStringList results = GeneralInputDialog::askForValues(values);
    if (results.size() > 0)//OK
    {
        name = results[0];
    }
    else//Cancel
    {
        return;
    }

    //ask for the location
    QString path = "";
    path = QtUtils::saveFileDialog("Please, select a path and a name to store the TestSuite:",
                                   ".",//TODO add dir reminder
                                   "*");
    if (path == "") return;

    //ask for the binary
    QString binaryPath = "";
    binaryPath = QtUtils::openFileDialog("Please, select the binary to be tested:",
                                         ".",//TODO add dir reminder
                                         "*");
    if (binaryPath == "") return;


    //check is is a valid binary
    bool ok = QtUtils::isExecutable(binaryPath);
    if (!ok)
    {
        QtUtils::newErrorDialog("The file selected is not a valid binary.");
        return;
    }

    //call the controller method
    ok = _processControl->newTestSuite(path.toStdString(),
                                      name.toStdString(),
                                      binaryPath.toStdString());

    //process checking
    if (!ok)
    {
        QtUtils::newErrorDialog("The TestSuite cannot be created.");
        return;
    }
}

void HMITesterControl::action_exit_triggered()
{
    DEBUG(D_GUI,"(HMITesterControl::action_exit_triggered)");
    exit(0);
}

void HMITesterControl::action_speed100_triggered()
{
    DEBUG(D_GUI,"(HMITesterControl::action_speed100_triggered)");
    _processControl->setExecutionSpeed(100);
}

void HMITesterControl::action_speed75_triggered()
{
    DEBUG(D_GUI,"(HMITesterControl::action_speed75_triggered)");
    _processControl->setExecutionSpeed(75);
}

void HMITesterControl::action_speed50_triggered()
{
    DEBUG(D_GUI,"(HMITesterControl::action_speed50_triggered)");
    _processControl->setExecutionSpeed(50);
}

void HMITesterControl::action_speed25_triggered()
{
    DEBUG(D_GUI,"(HMITesterControl::action_speed25_triggered)");
    _processControl->setExecutionSpeed(25);
}


void HMITesterControl::action_keepAlive_triggered(bool b)
{
    DEBUG(D_GUI,"(HMITesterControl::action_keepAlive_triggered)");
    _processControl->setContext().keepAlive = b;
}

void HMITesterControl::action_showTesterOnTop_triggered(bool b)
{
    DEBUG(D_GUI,"(HMITesterControl::action_showTesterOnTop_triggered)");
    _processControl->setContext().showTesterOnTop = b;

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


void HMITesterControl::PlayTestCaseSelected_triggered()
{
    DEBUG(D_GUI,"(HMITesterControl::PlayTestCaseSelected_triggered)");
    //nothing is performed here


}

void HMITesterControl::DeleteTestCaseSelected_triggered(bool)
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
    tsuiteMenu_->setEnabled ( true );

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
        QAction *action = NULL;

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
                    this, SLOT(DeleteTestCaseSelected_triggered(bool)));
        }

        //enabling menus
        tsuiteMenu_->setEnabled ( true );
        playTcaseMenu_->setEnabled ( true );
        deleteTcaseMenu_->setEnabled ( true );

        //selecting 1st testCase in play menu
        playTestCaseActionGroup_->actions().first()->setChecked(true);
    }
    //else disable the testSuite menu
    else
    {
        playTcaseMenu_->setEnabled ( false );
        deleteTcaseMenu_->setEnabled ( false );
    }

    //update the menu
    ui.menuBar->update();
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
    ui.menuBar->setVisible(true);
    //buttons
    _setEnableAndVisible(ui.tb_play,false);
    _setEnableAndVisible(ui.tb_pause,false);
    _setEnableAndVisible(ui.tb_stop,false);
    _setEnableAndVisible(ui.tb_rec,false);
    ui.tb_play->setVisible(true);
    ui.tb_rec->setVisible(true);
    //counter
    _setEnableAndVisible(ui.lcdNumber,false);
    _setEnableAndVisible(ui.lComp,false);;
    _setEnableAndVisible(ui.lItems,false);
    // window opacity
    setWindowOpacity(IDLE_OPACITY);
}

void HMITesterControl::setForm_stopState()
{
    emit form_stopState();
}

void HMITesterControl::_form_stopState()
{
    QtUtils::sleep(1000);
    //menu
    ui.menu_File->setEnabled(true);
    ui.menu_TestSuite->setEnabled(true);
    ui.menu_Config->setEnabled(true);
    ui.menuBar->setVisible(true);
    //buttons
    _setEnableAndVisible(ui.tb_play,true);
    _setEnableAndVisible(ui.tb_pause,false);
    _setEnableAndVisible(ui.tb_stop,false);
    _setEnableAndVisible(ui.tb_rec,true);
    //counter
    _setEnableAndVisible(ui.lcdNumber,false);
    ui.lcdNumber->display(0);
    _setEnableAndVisible(ui.lComp,false);
    _setEnableAndVisible(ui.lItems,false);
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
    _setEnableAndVisible(ui.tb_play,false);
    _setEnableAndVisible(ui.tb_pause,false);//TODO
    _setEnableAndVisible(ui.tb_stop,true);
    _setEnableAndVisible(ui.tb_rec,false);
    ui.tb_pause->setVisible(true);
    //counter
    _setEnableAndVisible(ui.lcdNumber,true);
    _setEnableAndVisible(ui.lComp,true);
    _setEnableAndVisible(ui.lItems,false);
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
    _setEnableAndVisible(ui.tb_play,false);
    _setEnableAndVisible(ui.tb_pause,false);//TODO
    _setEnableAndVisible(ui.tb_stop,true);
    _setEnableAndVisible(ui.tb_rec,false);
    ui.tb_pause->setVisible(true);
    //counter
    _setEnableAndVisible(ui.lcdNumber,true);
    _setEnableAndVisible(ui.lComp,false);
    _setEnableAndVisible(ui.lItems,true);
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
    ui.menu_Config->setEnabled(true);
    ui.menuBar->setVisible(true);
    //buttons
    _setEnableAndVisible(ui.tb_play,true);
    _setEnableAndVisible(ui.tb_pause,false);
    _setEnableAndVisible(ui.tb_stop,true);
    _setEnableAndVisible(ui.tb_rec,false);
    //counter
    _setEnableAndVisible(ui.lcdNumber,true);
    _setEnableAndVisible(ui.lComp,true);;
    _setEnableAndVisible(ui.lItems,false);
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
    ui.menuBar->setVisible(true);
    //buttons
    _setEnableAndVisible(ui.tb_play,false);
    _setEnableAndVisible(ui.tb_pause,false);
    _setEnableAndVisible(ui.tb_stop,true);
    _setEnableAndVisible(ui.tb_rec,true);
    //counter
    _setEnableAndVisible(ui.lcdNumber,true);
    _setEnableAndVisible(ui.lComp,false);
    _setEnableAndVisible(ui.lItems,true);
    // window opacity
    setWindowOpacity(IDLE_OPACITY);
}

//
////lcdCounter
///

//external method (launches signal)
void HMITesterControl::setLcdValueChanged(int i)
{
    emit changeLcdValue(i);
}

//internal method (change value)
void HMITesterControl::_changeLcdValue(int i)
{
    //lcd size adapter
    int aux = i;
    int count = log(aux) + 1;
    ui.lcdNumber->setNumDigits(count);

    //set value (progressive effect)
    aux = ui.lcdNumber->value();
    for(; aux <= i; aux+=1)
    {
        ui.lcdNumber->display(aux);
        QtUtils::sleep(7);//FIXME TODO whats is this?
    }
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
    target->setVisible(b);
}


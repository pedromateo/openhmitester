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
#include "debug.h"
#include "generalinputdialog.h"
#include "qtutils.h"
#include <QFile>
#include <QTest>
#include "ohtconfig.h"

HMITesterControl::HMITesterControl(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
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
    /// checking libraries
    ///

    QString libpath = QtUtils::getCurrentDir();
    DEBUG(D_GUI,libpath.toStdString());
    libpath = libpath + "/libPreload/";
    if ( ! ( QFile::exists ( libpath + "libLibPreload.so.1.0.0" ) &&
             QFile::exists ( libpath + "libLibPreload.so" ) &&
             QFile::exists ( libpath + "libLibPreload.so.1" ) &&
             QFile::exists ( libpath + "libLibPreload.so.1.0" ) ) )
    {
        DEBUG(D_GUI,"(HMITesterControl::initializeForm) Failed Preload Libraries checking.");
        QtUtils::newErrorDialog ( "Failed Preload Libraries checking." );
        assert ( 0 );
    }
    else
    {
        DEBUG(D_GUI,"(HMITesterControl::initializeForm) Preload Libraries checking OK.");
    }

    ///
    /// connecting signals and slots
    ///

    //buttons
    connect(ui.tb_play,SIGNAL(clicked()),this,SLOT(tb_play_clicked()));
    connect(ui.tb_pause,SIGNAL(clicked()),this,SLOT(tb_pause_clicked()));
    connect(ui.tb_stop,SIGNAL(clicked()),this,SLOT(tb_stop_clicked()));
    connect(ui.tb_rec,SIGNAL(clicked()),this,SLOT(tb_rec_clicked()));
    connect(ui.tb_screenshot,SIGNAL(clicked()),this,SLOT(tb_screenshot_clicked()));

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
    processControl_.GUIReference(this);
    processControl_.initialize();
    //set config values
    processControl_.setContext().keepAlive = false;
    processControl_.setContext().showTesterOnTop = true;
    processControl_.setExecutionSpeed(50);

    ///initialize visualization
    form_initState();

    //check always on top
    action_showTesterOnTop_triggered(true);
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
    if (processControl_.state() == ProcessControl::STOP)
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
        bool ok = processControl_.playTestCase(tcName.toStdString());
        if (!ok)
        {
            QtUtils::newErrorDialog ( "The selected Test Case is not valid." );
            return;
        }
    }

    //indicate that the play button has been pressed
    DEBUG(D_GUI,"(HMITesterControl::tb_play_clicked) Start Play or Resume process.");
    processControl_.playClicked();
}

void HMITesterControl::tb_pause_clicked()
{
    DEBUG(D_GUI,"(HMITesterControl::tb_pause_clicked)");
    processControl_.pauseClicked();
}

void HMITesterControl::tb_stop_clicked()
{
    DEBUG(D_GUI,"(HMITesterControl::tb_stop_clicked)");
    processControl_.stopClicked();
}

void HMITesterControl::tb_rec_clicked()
{
    DEBUG(D_GUI,"(HMITesterControl::tb_rec_clicked)");

    //if it is a new Test Case recording...
    if (processControl_.state() == ProcessControl::STOP)
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
        bool ok = processControl_.recordTestCase(data.toStdString());

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
                ok = processControl_.recordExistingTestCase(data.toStdString());
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
    processControl_.recClicked();
}

void HMITesterControl::tb_screenshot_clicked()
{
    //TODO
    DEBUG(D_GUI,"(HMITesterControl::tb_screenshot_clicked)");
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
    if (EXAMPLE_VALUES_ENABLED)
    {
        path = EXAMPLE_PATH;
    }
    else
    {
        path = QtUtils::openFileDialog("Please, select the file that contains the TestSuite:",
                                       ".",//TODO add dir reminder
                                       "*");
        if (path == "") return;
    }

    //open the testSuite
    bool ok = processControl_.openTestSuite(path.toStdString());
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
    if (EXAMPLE_VALUES_ENABLED)
    {
        path = EXAMPLE_PATH;
    }
    else
    {
        path = QtUtils::saveFileDialog("Please, select a path and a name to store the TestSuite:",
                                       ".",//TODO add dir reminder
                                       "*");
        if (path == "") return;
    }

    //ask for the binary
    QString binaryPath = "";
    if (EXAMPLE_VALUES_ENABLED)
    {
        binaryPath = EXAMPLE_BINARY;
    }
    else
    {
        binaryPath = QtUtils::openFileDialog("Please, select the binary to be tested:",
                                             ".",//TODO add dir reminder
                                             "*");
        if (binaryPath == "") return;
    }


    //check is is a valid binary
    bool ok = QtUtils::isExecutable(binaryPath);
    if (!ok)
    {
        QtUtils::newErrorDialog("The file selected is not a valid binary.");
        return;
    }

    //call the controller method
    ok = processControl_.newTestSuite(path.toStdString(),
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
    processControl_.setExecutionSpeed(100);
}

void HMITesterControl::action_speed75_triggered()
{
    DEBUG(D_GUI,"(HMITesterControl::action_speed75_triggered)");
    processControl_.setExecutionSpeed(75);
}

void HMITesterControl::action_speed50_triggered()
{
    DEBUG(D_GUI,"(HMITesterControl::action_speed50_triggered)");
    processControl_.setExecutionSpeed(50);
}

void HMITesterControl::action_speed25_triggered()
{
    DEBUG(D_GUI,"(HMITesterControl::action_speed25_triggered)");
    processControl_.setExecutionSpeed(25);
}


void HMITesterControl::action_keepAlive_triggered(bool b)
{
    DEBUG(D_GUI,"(HMITesterControl::action_keepAlive_triggered)");
    processControl_.setContext().keepAlive = b;
}

void HMITesterControl::action_showTesterOnTop_triggered(bool b)
{
    DEBUG(D_GUI,"(HMITesterControl::action_showTesterOnTop_triggered)");
    processControl_.setContext().showTesterOnTop = b;

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
    if (processControl_.deleteTestCase(tcName.toStdString()))
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
//constants big
const int b_x = 0;
const int b_y = 24;
const int b_width = 62;
const int b_height = 84;
//constants small
const int s_x = 0;
const int s_y = 24;
const int s_width = 62;
const int s_height = 84;


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
    ui.tb_play->setEnabled(false);
    ui.tb_pause->setEnabled(false);
    ui.tb_stop->setEnabled(false);
    ui.tb_rec->setEnabled(false);
    ui.tb_screenshot->setEnabled(false);
    //counter
    ui.lcdNumber->setEnabled(false);
    ui.lComp->setEnabled(false);
    ui.lItems->setEnabled(false);
    //size
    //setGeometry(s_x, s_y, s_width, s_height);
    setGeometry(geometry().x(),
                geometry().y(),
                s_width,
                s_height);
    setMaximumSize(s_width, s_height);
    setMinimumSize(s_width, s_height);
}

void HMITesterControl::setForm_stopState()
{
    emit form_stopState();
}

void HMITesterControl::_form_stopState()
{
    QTest::qWait (1000);
    //menu
    ui.menu_File->setEnabled(true);
    ui.menu_TestSuite->setEnabled(true);
    ui.menu_Config->setEnabled(true);
    ui.menuBar->setVisible(true);
    //buttons
    ui.tb_play->setEnabled(true);
    ui.tb_pause->setEnabled(false);
    ui.tb_stop->setEnabled(false);
    ui.tb_rec->setEnabled(true);
    ui.tb_screenshot->setEnabled(false);
    //counter
    ui.lcdNumber->setEnabled(false);
    ui.lcdNumber->display(0);
    ui.lComp->setEnabled(false);
    ui.lItems->setEnabled(false);
    //size
    //setGeometry(s_x, s_y, s_width, s_height);
    setGeometry(geometry().x(),
                geometry().y(),
                s_width,
                s_height);
    setMaximumSize(s_width, s_height);
    setMinimumSize(s_width, s_height);
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
    ui.tb_play->setEnabled(false);
    ui.tb_pause->setEnabled(false);//TODO
    ui.tb_stop->setEnabled(true);
    ui.tb_rec->setEnabled(false);
    ui.tb_screenshot->setEnabled(false);
    //counter
    ui.lcdNumber->setEnabled(true);
    ui.lComp->setEnabled(true);
    ui.lItems->setEnabled(false);
    //size
    //setGeometry(s_x, s_y, s_width, s_height);
    setGeometry(geometry().x(),
                geometry().y(),
                s_width,
                s_height);
    setMaximumSize(b_width, b_height);
    setMinimumSize(b_width, b_height);
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
    ui.tb_play->setEnabled(false);
    ui.tb_pause->setEnabled(false);//TODO
    ui.tb_stop->setEnabled(true);
    ui.tb_rec->setEnabled(false);
    ui.tb_screenshot->setEnabled(false);
    //counter
    ui.lcdNumber->setEnabled(true);
    ui.lComp->setEnabled(false);
    ui.lItems->setEnabled(true);
    //size
    //setGeometry(s_x, s_y, s_width, s_height);
    setGeometry(geometry().x(),
                geometry().y(),
                s_width,
                s_height);
    setMaximumSize(b_width, b_height);
    setMinimumSize(b_width, b_height);
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
    ui.tb_play->setEnabled(true);
    ui.tb_pause->setEnabled(false);
    ui.tb_stop->setEnabled(true);
    ui.tb_rec->setEnabled(false);
    ui.tb_screenshot->setEnabled(false);//TODO
    //counter
    ui.lcdNumber->setEnabled(true);
    ui.lComp->setEnabled(true);
    ui.lItems->setEnabled(false);
    //size
    //setGeometry(s_x, s_y, s_width, s_height);
    setGeometry(geometry().x(),
                geometry().y(),
                s_width,
                s_height);
    setMaximumSize(b_width, b_height);
    setMinimumSize(b_width, b_height);
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
    ui.tb_play->setEnabled(false);
    ui.tb_pause->setEnabled(false);
    ui.tb_stop->setEnabled(true);
    ui.tb_rec->setEnabled(true);
    ui.tb_screenshot->setEnabled(false);//TODO
    //counter
    ui.lcdNumber->setEnabled(true);
    ui.lComp->setEnabled(false);
    ui.lItems->setEnabled(true);
    //size
    //setGeometry(s_x, s_y, s_width, s_height);
    setGeometry(geometry().x(),
                geometry().y(),
                s_width,
                s_height);
    setMaximumSize(b_width, b_height);
    setMinimumSize(b_width, b_height);
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
    for(aux; aux <= i; aux+=1)
    {
        ui.lcdNumber->display(aux);
        QTest::qWait(7);
    }
}


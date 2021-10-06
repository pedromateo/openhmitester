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
#ifndef HMITESTERCONTROL_H
#define HMITESTERCONTROL_H

#include <ui_hmitestercontrol.h>
#include <processcontrol.h>

#include <QMainWindow>
#include <QSettings>



class HMITesterControl : public QMainWindow
{
    Q_OBJECT

public:
    HMITesterControl(PreloadingAction* pa, DataModelAdapter* dma, QWidget *parent = 0);
    ~HMITesterControl();

public:
    //test suite info update
    void updateTestSuiteInfo(DataModel::TestSuite*);

    // Automation, once command
    void automationRunTestSuite(QString path);

    //form visualization and state
    void setForm_initState();
    void setForm_stopState();
    void setForm_playState();
    void setForm_recState();
    void setForm_pausePlayState();
    void setForm_pauseRecState();
    void setForm_playbackStatus(int);
    void setForm_recordingStatus(int);


    //handle with command line
    void openFileAndSetupData(QString pathConfig);
    void playAllTestCase();

private:
    Ui::HMITesterWindow ui;

    // initialization
    void _initializeForm();
    void _initializeMenu();

    // supporting
    void _setEnableAndVisible(QWidget* target, bool b);

    //menus
    QMenu *_menu_Main;
    QMenu *_menu_File;
    QMenu *_menu_Config;
    QMenu *_menu_Speed;
    QActionGroup *_speedActionGroup;
    QSettings _settings;


    QMenu* _playlistMenu;
    QActionGroup* _playlistMenu_agroup;

    // overall process controller
    ProcessControl* _processControl;

    // kill hmitester after finished test
    bool _keepAlive;

private slots:
    //button handlers
    void tb_play_clicked();
    void tb_pause_clicked();
    void tb_stop_clicked();
    void tb_rec_clicked();

    //menu actions handler
    void action_open_triggered();
    void action_edit_triggered();
    void action_new_triggered();
    void action_close_triggered();
    void action_exit_triggered();
    void action_speed1x_triggered();
    void action_speed05x_triggered();
    void action_speed2x_triggered();
    void action_speed4x_triggered();
    void action_keepAlive_triggered(bool);
    void action_showTesterOnTop_triggered(bool);

    //global shortcut
    void action_global_shortcut_triggered();

    //testSuite handling
    void _playTestCaseSelected_triggered(bool);

    //form visualization internal methods
    void _form_initState();
    void _form_stopState();
    void _form_playState();
    void _form_recState();
    void _form_pausePlayState();
    void _form_pauseRecState();
    void _form_playbackStatus(int);
    void _form_recordingStatus(int);

    // status bar messages
    void _set_statusbar_text(const QString &msg, int timeout = 0);
    void _set_statusbar_text(const char* msg, int timeout = 0);

    void _test_ended_handle();

signals:

    ///form visualization signals
    void form_initState();
    void form_stopState();
    void form_playState();
    void form_recState();
    void form_playbackStatus(int);
    void form_recordingStatus(int);
    void form_pausePlayState();
    void form_pauseRecState();
};

#endif // HMITESTERCONTROL_H

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

#if QT_VERSION >= 0x050000 // Qt5 code
#include <QtWidgets>
#else // Qt4 code
#include <QtGui/QMainWindow>
#endif




class HMITesterControl : public QMainWindow
{
    Q_OBJECT

public:
    HMITesterControl(PreloadingAction* pa, DataModelAdapter* dma, QWidget *parent = 0);
    ~HMITesterControl();

public:
    //test suite info update
    void updateTestSuiteInfo(DataModel::TestSuite*);

    //form visualization and state
    void setForm_initState();
    void setForm_stopState();
    void setForm_playState();
    void setForm_recState();
    void setForm_pausePlayState();
    void setForm_pauseRecState();

    //external method to change lcd value (because threads)
    //emits a signal to launch the internal
    void setLcdValueChanged(int);



private:
    Ui::HMITesterWindow ui;

    // initialization
    void _initializeForm();

    // supporting
    void _setEnableAndVisible(QWidget* target, bool b);

    //menus
    QMenu *mainMenu_;
    QMenu *fileMenu_;
    QMenu *tsuiteMenu_;
    QMenu *playTcaseMenu_;
    QMenu *deleteTcaseMenu_;
    QMenu *configMenu_;
    QMenu *speedMenu_;
    QActionGroup *playTestCaseActionGroup_;
    QActionGroup *deleteTestCaseActionGroup_;
    QActionGroup *speedActionGroup_;

    // overall process controller
    ProcessControl* _processControl;

private slots:
    //button handlers
    void tb_play_clicked();
    void tb_pause_clicked();
    void tb_stop_clicked();
    void tb_rec_clicked();

    //menu actions handler
    void action_open_triggered();
    void action_new_triggered();
    void action_exit_triggered();
    void action_speed100_triggered();
    void action_speed75_triggered();
    void action_speed50_triggered();
    void action_speed25_triggered();
    void action_keepAlive_triggered(bool);
    void action_showTesterOnTop_triggered(bool);

    //testSuite handling
    void PlayTestCaseSelected_triggered();
    void DeleteTestCaseSelected_triggered(bool);

    //form visualization internal methods
    void _form_initState();
    void _form_stopState();
    void _form_playState();
    void _form_recState();
    void _form_pausePlayState();
    void _form_pauseRecState();

    //internal method to change lcd value (because threads)
    void _changeLcdValue(int);

signals:

    ///form visualization signals
    void form_initState();
    void form_stopState();
    void form_playState();
    void form_recState();
    void form_pausePlayState();
    void form_pauseRecState();

    //a signal to change the lcd widget
    void changeLcdValue(int);
};

#endif // HMITESTERCONTROL_H

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
#ifndef ITEMMANAGER_H
#define ITEMMANAGER_H

#include "comm.h"
#include "datamodel.h"
#include "recordingobserver.h"
#include <QObject>

class ItemManager : public QObject
{
    Q_OBJECT

    public:

    ItemManager(Comm*, RecordingObserver*);
    ~ItemManager();

public slots:

    ///recording process control
    void recordTestCase(DataModel::TestCase*);
    void pauseRecording();
    void resumeRecording();
    void stopRecording();

    void applicationFinished();

    //recording process state
    bool isRecording();
    bool isPaused();

    //messages received from Preload Module
    void handleNewTestItem ( DataModel::TestItem* );

private:

    //recording flags
    bool f_recording_;
    bool f_paused_;
    bool f_terminate_;
    bool f_error_;

    //received test items counter
    int rtiCounter_;

    //communication
    Comm *comm_;

    //observer
    RecordingObserver *observer_;

    //current test case
    DataModel::TestCase *currentTestCase_;
};

#endif // ITEMMANAGER_H

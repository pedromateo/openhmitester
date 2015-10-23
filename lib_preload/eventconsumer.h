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
#ifndef EVENTCONSUMER_H
#define EVENTCONSUMER_H

#include <datamodel.h>
#include <QObject>

class EventConsumer : public QObject
{
    Q_OBJECT

public:

    EventConsumer();

    ///
    /// this method is called to install the event consumer
    ///
    virtual void install() = 0;

    ///
    /// this method is called when the consumer wants to
    /// send a new item to the HMI Tester
    ///
    void sendNewTestItem(DataModel::TestItem& ti);

    ///
    /// capture process control methods
    ///
    virtual void startCapture() = 0;
    virtual void pauseCapture() = 0;
    virtual void resumeCapture() = 0;
    virtual void stopCapture() = 0;

private:

signals:

    ///
    ///
    ///
    void newTestItem(const DataModel::TestItem&);
};

#endif // EVENTCONSUMER_H

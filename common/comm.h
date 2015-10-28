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
#ifndef COMM_H
#define COMM_H

#include <messageclientserver.h>
#include <datamodel.h>
#include <utilclasses.h>
#include <QTcpServer>
#include <QTcpSocket>
#include <memory>
#include <deque>


class Comm : public QObject
{
    Q_OBJECT

public:
    Comm ( int port, bool isServer );

    MessageClientServer * messageServer();
    MessageClientServer * messageClient();

public slots:

    void handleSendTestItem (const DataModel::TestItem&);
    //DataModel::TestItem* handleReceivedTestItem ( const std::string& msg );

    void handleSendMessage ( const QString& );//from client class (input method)
    void handleSendMessage ( const std::string& );//from client class (input method)
    void handleReceivedMessage ( const QString& );//internal

    //handles the client connection
    void handleNewClientConnected();
    void handleClientDisconnected();

    //handles errors
    void handleError ( const QString& );

signals:
    void receivedTestItem (DataModel::TestItem*);
    void receivedMessage ( const QString& );//to client class (output method)
    void sendMessage ( const QString& );//internal
    void error ( const std::string& );

private:

    // Pending events to process
    std::deque<DataModel::TestItem> testItemQueue_;
    std::auto_ptr<MessageClientServer> mcs_;
    bool isServer_;
    bool clientConnected_;
};


#endif // COMM_H

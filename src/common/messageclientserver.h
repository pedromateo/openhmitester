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
#ifndef MESSAGECLIENTSERVER_H
#define MESSAGECLIENTSERVER_H

#include <datamodel.h>
#include <utilclasses.h>
#include <QTcpServer>
#include <QTcpSocket>
#include <memory>
#include <deque>


class MessageClientServer : public QTcpServer
{
    Q_OBJECT

public:
    MessageClientServer ( QObject *parent, uint port, bool isServer );
    ~MessageClientServer();

public slots:
    void readMessage();
    void writeMessage ( const QString& );
    void displayError ( QAbstractSocket::SocketError socketError );

    void handleClientDisconnected();

signals:
    void receivedMessage ( const QString& );
    void error ( const QString& );
    void newClientConnected();
    void clientDisconnected();

protected:
#if QT_VERSION >= 0x050000
    void incomingConnection ( qintptr socketDescriptor );
#else
    void incomingConnection (int socketDescriptor );
#endif


    bool _readSocket ( QTcpSocket *s, QCircularByteArray_ *buffer );
    bool _writeSocket ( QTcpSocket *s, const QString& message );

private:

    uint _port;

    std::auto_ptr<QTcpSocket> currentSocket_;

    QCircularByteArray_ buffer_;
};



/// ////////////////////////////////////////////
/// ////////////////////////////////////////////
/// ////////////////////////////////////////////

class ClientSocket : public QTcpSocket
{
    Q_OBJECT

public:
    ClientSocket ( int sock, QObject *parent );
    ~ClientSocket();

signals:
    void error ( const QString& );

};


#endif // MESSAGECLIENTSERVER_H

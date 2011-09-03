/***************************************************************************
 *   Copyright (C) 2011 by Mike Achtelik                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/


#ifndef PORT_H
#define PORT_H

#include <QTcpSocket>
#include <QObject>
#include <QTimer>

#include "packet.h"
#include "dispatcher.h"

class TCPPort: public QObject
{
    Q_OBJECT
public:
    TCPPort(Dispatcher *dispatcher,QObject * parent = 0);
    ~TCPPort();
protected slots:
    void recive();
    void handleError(QAbstractSocket::SocketError);
public slots:
    virtual void setPort(int port) = 0;
signals:
    void connectionError();
protected:
    Dispatcher *m_dispatcher;
    QTcpSocket *m_socket;
    QByteArray m_buffer;
    int m_port;
};

#endif // PORT_H


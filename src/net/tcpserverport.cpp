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

#include "tcpserverport.h"

#include "settings.h"

TCPServerPort::TCPServerPort(Dispatcher *dispatcher, QObject* parent):
        TCPPort(dispatcher,parent),
        m_serversocket(new QTcpServer(this))
{
    m_socket=0;
    m_serversocket->listen(QHostAddress::Any,Settings::port());
    connect(m_serversocket,SIGNAL(newConnection()),this,SLOT(connected()));
}

TCPServerPort::~TCPServerPort()
{

}

void TCPServerPort::setPort(int port)
{
    m_port=port;
    m_serversocket->close();
    m_serversocket->listen(QHostAddress::Any,port);
}

void TCPServerPort::connected()
{
    if (m_serversocket->hasPendingConnections()) {
        m_socket=m_serversocket->nextPendingConnection();
        connect(m_socket,SIGNAL(readyRead()),this,SLOT(recive()));
        //connect(m_socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(handleError(QAbstractSocket::SocketError)));
    }
}

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


#include "tcpport.h"

TCPPort::TCPPort(Dispatcher *dispatcher, QObject* parent):
        QObject(parent),
        m_dispatcher(dispatcher),
        packetSeparator(31)
{
}

TCPPort::~TCPPort() {
    delete m_socket;
}

void TCPPort::recive() {
    while (m_socket->bytesAvailable()) {
        m_buffer.append(m_socket->readAll());
    }
    QList<QByteArray> data = m_buffer.split(packetSeparator);
    if (data.size()>1) {
        for (int i = 0;i<data.size()-1;++i) {
            m_dispatcher->dispatch(Packet(data.at(i)));
        }
        m_buffer.clear();
        if (data.last().size()>0) {
            m_buffer.append(data.last());
        }
    }
}

void TCPPort::handleError(QAbstractSocket::SocketError )
{
    if (QAbstractSocket::HostNotFoundError) {
        emit connectionError();
    }
}

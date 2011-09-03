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

#include "tcpclientport.h"

TCPClientPort::TCPClientPort(Dispatcher *dispatcher, QObject* parent):
        TCPPort(dispatcher,parent)
{
    m_socket = new QTcpSocket(this);
    connect(m_socket,SIGNAL(readyRead()),this,SLOT(recive()));
    connect(m_socket,SIGNAL(connected()),this,SLOT(startTransfer()));
    connect(m_socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(handleError(QAbstractSocket::SocketError)));
    connect(m_socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(clearBuffer()));
    connect(m_dispatcher,SIGNAL(closeConnection()),this,SLOT(disconnectSocket()));
}

TCPClientPort::~TCPClientPort()
{

}

void TCPClientPort::setPort(int port) {
    m_port=port;
}


void TCPClientPort::setIp(QString ip)
{
    this->m_ip=ip;
}

void TCPClientPort::connectSocket()
{
    qDebug()<<"Connecting..";
    m_socket->connectToHost(m_ip,m_port);
}

void TCPClientPort::disconnectSocket()
{
    m_socket->close();
}


void TCPClientPort::send(Packet &packet) {
    packetBuffer.append(packet);
    connectSocket();
}

void TCPClientPort::clearBuffer()
{
    m_buffer.clear();
    packetBuffer.clear();
}


void TCPClientPort::startTransfer()
{
    while (!packetBuffer.isEmpty()) {
        m_socket->write(packetBuffer.takeFirst().toByteArray());
        qDebug()<<"Sending Packet";
    }
}

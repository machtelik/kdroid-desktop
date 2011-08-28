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


#include "port.h"

Port::Port(QObject *parent):
        QObject(parent),
        m_retryCount(0),
        m_retryTimer(new QTimer()),
        socket(new QUdpSocket())
{
    connect(this,SIGNAL(pong()),this,SLOT(recivedPong()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(recive()));

    connect(m_retryTimer, SIGNAL(timeout()), this, SLOT(send()));
    m_retryTimer->setInterval(3000);

}

Port::~Port() {
    delete socket;
    delete m_retryTimer;
}

void Port::send(Packet &packet) {
    m_packets.append(packet);
    if (!m_retryTimer->isActive()) {
        send();
        m_retryTimer->start();
    }
}

void Port::send() //Sometimes my HTC Hero doesnt recive the packet. Resend it a few times
{
    qDebug()<<"Sending Packet";
    ++m_retryCount;
    socket->writeDatagram(m_packets.first().toByteArray(),QHostAddress(ip),port);
    if (m_retryCount>4) {
        m_retryCount=0;
        m_packets.clear();
        emit connectionError();
        m_retryTimer->stop();
    }
}


void Port::recivedPong()
{
    qDebug()<<"Recived Pong";
    m_retryCount=0;
    m_retryTimer->stop();
    if (!m_packets.isEmpty()) {
        m_packets.removeFirst();
    }
}


void Port::setPort(int Port) {
    port=Port;
    socket->close();
    socket->bind(Port);
}


void Port::setIp(QString ip)
{
    this->ip=ip;
}


void Port::recive() {
    while (socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        socket->readDatagram(datagram.data(), datagram.size(),&sender, &senderPort);

        dispatch( Packet(datagram) );
    }
}


void Port::dispatch(Packet packet) {
    if (packet.getType()=="SMS") {
        emit newSMSMessage(packet.toSMSMessage());
        return;
    }
    if (packet.getType()=="Contact") {
        emit newContact(packet.toContact());
        return;
    }
    if (packet.getType()=="Status") {
        if (packet.getFirstArgument()=="Pong") {
            emit pong();
            return;
        }
        if (packet.getFirstArgument()=="AckGetAll") {
            emit ackGetAll();
            return;
        }
        if (packet.getFirstArgument()=="DoneGetAll") {
            emit doneGetAll();
            return;
        }
        if (packet.getFirstArgument()=="SMSSend") {
            emit SMSSend();
            return;
        }
    }
    qDebug()<<"Unknown Packet: "<<packet.getType();

}

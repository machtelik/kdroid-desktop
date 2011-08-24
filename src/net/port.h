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

#include <QUdpSocket>
#include <QObject>
#include <QTimer>

#include "packet.h"

class Port: public QObject
{
  Q_OBJECT
public:
    Port(QObject * parent = 0);
    ~Port();
    void send(Packet &packet);
    void setPort(int Port);
    void setIp(QString ip);
private slots:
    void recive();
    void recivedPong();
    void send();
signals:
    void newSMSMessage(SMSMessage message);
    void newContact(Contact contact);
    void doneGetAll();
    void ackGetAll();
    void pong();
    void connectionError();
    void SMSSend();
private:
    void dispatch(Packet packet);
    int port;
    QString ip;
    int m_retryCount;
    QTimer *m_retryTimer;
    QList<Packet> m_packets;
    QUdpSocket *socket;
};

#endif // PORT_H


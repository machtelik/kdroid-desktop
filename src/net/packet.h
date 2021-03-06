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


#ifndef PACKET_H
#define PACKET_H

#include <QStringList>
#include <QString>
#include <QChar>
#include <QObject>

#include "../sms/smsmessage.h"
#include "../contact/contact.h"

#define packetSeparator 31
#define seperator 30

class Packet
{
public:
    Packet(QString type);
    Packet(QByteArray data);
    Packet(SMSMessage message);
    ~Packet();
    QString getType();
    const QStringList* getArguments();
    QString getFirstArgument();
    void addArgument(QString argument);
    SMSMessage toSMSMessage();
    Contact toContact();
    QByteArray toByteArray();

protected:
    QStringList arguments;
    QString type;
};

#endif // PACKET_H


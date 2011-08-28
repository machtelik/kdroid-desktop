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

#include "packet.h"

#include <QStringList>
#include <QDebug>

Packet::Packet(QString type):
        seperator(30)
{
    this->type=type;
}

Packet::Packet(QByteArray data):
        seperator(30)
{

    QStringList list = QString::fromUtf8(data).split(seperator);
    type=list.at(0);
    for (int i=1;i<list.size();++i) {
        arguments.append(list.at(i));
    }
}

Packet::Packet(SMSMessage message):
seperator(30)
{
    type="SMS";
    addArgument(QString::number(message.Id));
    addArgument(message.Address);
    addArgument(message.Body);
    addArgument(QString::number(message.Time));
    addArgument(message.Type);
}


Packet::~Packet() {
}

QString Packet::getType() {
    return type;
}

const QStringList* Packet::getArguments() {
    return &arguments;
}

QString Packet::getFirstArgument() {
    return arguments.at(0);
}

void Packet::addArgument(QString argument) {
    arguments.append(argument.toUtf8());
}

SMSMessage Packet::toSMSMessage() {
    SMSMessage message;
    message.Id=arguments.at(0).toInt();
    message.Address=arguments.at(1);
    message.Body=arguments.at(2);
    message.Time=arguments.at(3).toLong();
    message.Type=arguments.at(4);
    return message;
}

Contact Packet::toContact()
{
  Contact contact;
  contact.Id=arguments.at(0).toInt();
  contact.Name=arguments.at(1);
  contact.Address=arguments.at(2);
  return contact;
}


QByteArray Packet::toByteArray()
{
    QByteArray bytes;
    bytes.append(type);
    bytes.append(seperator);
    for (int i = 0; i<arguments.size();++i) {
        bytes.append(arguments.at(i));
        bytes.append(seperator);
    }
    return bytes;
}




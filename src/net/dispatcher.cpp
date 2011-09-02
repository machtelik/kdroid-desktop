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

#include "dispatcher.h"

#include <QDebug>

Dispatcher::Dispatcher(QObject* parent):
        QObject(parent)
{

}

void Dispatcher::dispatch(Packet packet) {
    if (packet.getType()=="SMS") {
        emit newSMSMessage(packet.toSMSMessage());
        return;
    }
    if (packet.getType()=="Contact") {
        emit newContact(packet.toContact());
        return;
    }
    if (packet.getType()=="Status") {
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
        if (packet.getFirstArgument()=="newMessage") {
            emit newSMSMessage();
            return;
        }
        if (packet.getFirstArgument()=="end") {
            emit closeConnection();
            return;
        }
    }
    qDebug()<<"Unknown Packet: "<<packet.getType();
    if(packet.getArguments()->size()>0) {
      qDebug()<<"Packet Data: "<<packet.getFirstArgument();
    }
}





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

#include "messagedbushandler.h"
#include "MessageDBusHandlerAdaptor.h"

#include <QDBusConnection>

#include "../view/kdroidxmlgui.h"


MessageDBusHandler::MessageDBusHandler(KDroid* parent):
        QObject(parent),
        m_app(parent)
{
    new MessageDBusHandlerAdaptor(this);

    QDBusConnection dbus = QDBusConnection::sessionBus();

    dbus.registerObject("/Message", this);

    dbus.registerService("org.kde.kdroid");
}

void MessageDBusHandler::sendMessage(const QString& address, const QString& body)
{
    m_app->sendSMS(address,body);
}

void MessageDBusHandler::showAddress(const QString& address)
{
    m_app->getMainWindow()->getSendView()->setAddress(address);
}

void MessageDBusHandler::showBody(const QString& body)
{
    m_app->getMainWindow()->getSendView()->setBody(body);
}

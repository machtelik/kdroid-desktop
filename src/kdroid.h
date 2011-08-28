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

#ifndef KDROID_H
#define KDROID_H

#include <KUniqueApplication>
#include "view/kdroidxmlgui.h"
#include "sms/smsmessage.h"
#include "net/port.h"
#include "sms/smslist.h"
#include "contact/contactlist.h"
#include "xmlhandler.h"

#include <KCmdLineArgs>

class KDroidXmlGui;

class KDroid : public KUniqueApplication
{
Q_OBJECT
public:
    KDroid();
    virtual ~KDroid();

    int newInstance();
public slots:
    void SyncSms();
    void sendSMS(SMSMessage message);
    void settingsChanged();
private slots:
    void SMSSend();
    void SyncComplete();
    void showNotification(QString title, QString message, QString type);
    void ackGetAll();
    void noConnection();
    void newMessage(SMSMessage message);
public:
    XMLHandler* getXMLHandler();
    ContactList* getContactList();
    SMSList* getSMSList();
    Port* getPort();
private:
    KNotification* m_notify;
    QTimer *m_timer;
    Port *m_port;
    SMSList *m_smslist;
    ContactList *m_contactlist;
    XMLHandler *m_xmlhandler;
    KDroidXmlGui *m_gui;
    QString m_saveLocation;

    void handleArgs(KCmdLineArgs *args);
    void activateFirstContact();
};

#endif // KDROID_H

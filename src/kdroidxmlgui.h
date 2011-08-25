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


#include <KXmlGuiWindow>
#include <QList>
#include <QTreeWidgetItem>
#include <QTimer>
#include <KNotification>
#include <KSystemTrayIcon>
#include <QModelIndex>
#include <KAction>

#include "ui_prefs_base.h"
#include "view/sendview.h"
#include "net/port.h"
#include "sms/smslist.h"
#include "contact/contactlist.h"
#include "xmlhandler.h"

class KDroidView;
class KToggleAction;
class KUrl;

class KDroid : public KXmlGuiWindow
{
    Q_OBJECT
public:

    KDroid();
    virtual ~KDroid();

private slots:
    void SyncSms();
    void optionsPreferences();
    void showNotification(QString title, QString message, QString type);
    void settingsChanged();
    void closeEvent(QCloseEvent *event);
    void xmlExport();
    void selectionChanged(QModelIndex index);
    void ackGetAll();
    void noConnection();
    void sendSMS(SMSMessage message);
    void SMSSend();
    void SyncComplete();

private:
    void setupActions();

    KNotification* m_notify;

    Ui::prefs_base ui_prefs_base ;
    KDroidView *m_view;
    SendView *m_sendview;
    KSystemTrayIcon *m_trayIcon;
    QTimer *m_timer;
    Port *m_port;
    SMSList *m_smslist;
    ContactList *m_contactlist;
    XMLHandler *m_xmlhandler;

    KToggleAction *m_toolbarAction;
    KToggleAction *m_statusbarAction;
    KAction *sync;

    QString m_saveLocation;
};

#endif // _KDROID_H_

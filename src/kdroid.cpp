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

#include "kdroid.h"

#include <KCmdLineArgs>
#include <KNotification>
#include <KStandardDirs>
#include <KStartupInfo>
#include <KActionCollection>

#include <QDebug>

#include "settings.h"

#include "view/kdroidview.h"

KDroid::KDroid():
        m_timer ( new QTimer() ),
        m_dispatcher(new Dispatcher(this)),
        m_clientport ( new TCPClientPort ( m_dispatcher,this ) ),
        m_serverport(new TCPServerPort(m_dispatcher,this)),
        m_smslist ( new SMSList ( this ) ),
        m_contactlist ( new ContactList ( this ) ),
        m_xmlhandler ( new XMLHandler ( m_contactlist,m_smslist,this ) ),
        m_gui(new KDroidXmlGui(this)),
        m_statusNotifier(new KStatusNotifierItem(this)),
        m_messageDBusHandler(new MessageDBusHandler(this))
{
    m_saveLocation = KStandardDirs().saveLocation ( "data",qAppName() ).append ( "kdroidData.xml" );

    connect ( m_dispatcher,SIGNAL ( newSMSMessage ( SMSMessage ) ),m_smslist,SLOT ( addSMS ( SMSMessage ) ) );
    connect ( m_dispatcher,SIGNAL ( newContact ( Contact ) ),m_contactlist,SLOT ( addContact ( Contact ) ) );
    connect ( m_dispatcher,SIGNAL ( ackGetAll() ),this,SLOT ( ackGetAll() ) );
    connect ( m_dispatcher,SIGNAL ( SMSSend() ),this,SLOT ( SMSSend() ) );
    connect ( m_dispatcher,SIGNAL ( doneGetAll() ),this,SLOT ( SyncComplete() ) );
    connect ( m_dispatcher,SIGNAL ( newSMSMessage () ),this,SLOT ( newMessage() ) );

    connect ( m_clientport,SIGNAL ( connectionError() ),this,SLOT ( noConnection() ) );
    connect ( m_serverport,SIGNAL ( connectionError() ),this,SLOT ( noConnection() ) );

    connect ( m_smslist,SIGNAL ( newContact ( QString,long ) ),m_contactlist,SLOT ( updateContacts ( QString,long ) ) );

    connect ( m_timer, SIGNAL ( timeout() ), this, SLOT ( SyncSms() ) );

    if ( m_xmlhandler->load ( m_saveLocation ) )
    {
        activateFirstContact();
    }

    m_statusNotifier->setIconByName("pda");
    m_statusNotifier->setCategory(KStatusNotifierItem::Communications);
    m_statusNotifier->setStatus(KStatusNotifierItem::Passive);
    m_statusNotifier->setAssociatedWidget(m_gui);

    settingsChanged();
}

KDroid::~KDroid()
{
    if (m_gui) {
        delete m_gui;
    }
    if (m_statusNotifier) {
        delete m_statusNotifier;
    }
    qDebug()<<"Bye Bye";
}

int KDroid::newInstance()
{
    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    handleArgs(args);
    static bool first = true;
    if (!first) {
        KStartupInfo::setNewStartupId(m_gui, startupId());
    }
    first = false;
    args->clear();
    return 0;
}

void KDroid::handleArgs(KCmdLineArgs* args)
{
    if (!args->isSet("quiet")) {
        m_gui->show();
    }
    if (args->isSet("test")) {
        Packet p = Packet(QString("Status"));
        p.addArgument("connectionTest");
        m_clientport->send(p);
    }
    if (!args->isSet("send")) {
        if (args->isSet("address")) {
            m_gui->getSendView()->setAddress(args->getOption("address"));
        }
        if (args->isSet("body")) {
            m_gui->getSendView()->setBody(args->getOption("body"));
        }
    }
    if (args->isSet("send") && args->isSet("address") && args->isSet("body")) {
        sendSMS(args->getOption("address"),args->getOption("body"));
    }
}

void KDroid::newMessage()
{
    showNotification ( i18n ( "New Message" ),"newMessage" );
    qDebug() <<"new Message";
    m_xmlhandler->save ( m_saveLocation );

    activateFirstContact();
}

void KDroid::sendSMS ( SMSMessage message )
{
    message.Time=QDateTime::currentMSecsSinceEpoch();
    message.Type="Send";
    Packet packet = Packet ( message );
    m_clientport->send ( packet );
}

void KDroid::sendSMS(QString address, QString body)
{
    SMSMessage message;
    message.Address=address;
    message.Body=body;
    sendSMS(message);
}


void KDroid::SMSSend()
{
    showNotification ( i18n ( "SMS send" ),"sendMessage" );
    m_gui->getSendView()->setBody("");
    qDebug() <<"Message Send";
}

void KDroid::SyncComplete()
{
    qDebug() <<"Sync Complete";
    showNotification ( i18n ( "Sync complete" ),"syncComplete" );

    activateFirstContact();

    m_xmlhandler->save ( m_saveLocation );

    m_gui->setEnableSyncButton(true);

}

void KDroid::SyncSms()
{
    qDebug() <<"Sync start";
    m_gui->setEnableSyncButton(false);
    if ( Settings::auto_sync() && !m_timer->isActive() )
    {
        m_timer->setInterval ( Settings::timer_interval() *60000 );
        m_timer->start();
    }
    Packet packet =  Packet ( QString ( "Request" ) );
    packet.addArgument ( "getAll" );
    m_clientport->send ( packet );
}

void KDroid::ackGetAll()
{
    qDebug() <<"Sync Ack";
    m_smslist->clear();
    m_contactlist->clear();
}

void KDroid::noConnection()
{
    qDebug() <<"No connection";
    m_gui->setEnableSyncButton(true);
    if ( m_timer->isActive() )
    {
        m_timer->stop();
    }
    showNotification ( i18n ( "Device unreachable" ),"noConnection" );
}

void KDroid::showNotification ( QString message, QString type )
{
    KNotification::event(type,message,SmallIcon ( "pda", KIconLoader::SizeMedium ),m_gui);
}

void KDroid::settingsChanged()
{
    qDebug() <<"Settings Changed";
    m_clientport->setPort ( Settings::port() );
    m_clientport->setIp ( Settings::ip_address() );
    m_serverport->setPort(Settings::port());
    if ( Settings::auto_sync() )
    {
        m_timer->setInterval ( Settings::timer_interval() *60000 );
        m_timer->start();
        SyncSms();
    }
    else
    {
        m_timer->stop();
    }

}

void KDroid::activateFirstContact()
{
    m_smslist->filter ( m_contactlist->getFirstAddress() );
    QListView* smsList = m_gui->getMainView()->getContactListView();
    smsList->setCurrentIndex(smsList->indexAt(QPoint(0,0)));
    m_gui->getSendView()->setAddress(m_contactlist->getFirstAddress());
}


XMLHandler* KDroid::getXMLHandler()
{
    return m_xmlhandler;
}

ContactList* KDroid::getContactList()
{
    return m_contactlist;
}

SMSList* KDroid::getSMSList()
{
    return m_smslist;
}

TCPPort* KDroid::getPort()
{
    return m_clientport;
}

KDroidXmlGui* KDroid::getMainWindow()
{
    return m_gui;
}





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

#include <QDebug>

#include "settings.h"

KDroid::KDroid():
        m_timer ( new QTimer() ),
        m_port ( new Port ( this ) ),
        m_smslist ( new SMSList ( this ) ),
        m_contactlist ( new ContactList ( this ) ),
        m_xmlhandler ( new XMLHandler ( m_contactlist,m_smslist,this ) ),
        m_gui(new KDroidXmlGui(this))
{

    m_saveLocation = KStandardDirs().saveLocation ( "data",qAppName() ).append ( "kdroidData.xml" );

    connect ( m_port,SIGNAL ( newSMSMessage ( SMSMessage ) ),m_smslist,SLOT ( addSMS ( SMSMessage ) ) );
    connect ( m_port,SIGNAL ( newContact ( Contact ) ),m_contactlist,SLOT ( addContact ( Contact ) ) );
    connect ( m_port,SIGNAL ( ackGetAll() ),this,SLOT ( ackGetAll() ) );
    connect ( m_port,SIGNAL ( connectionError() ),this,SLOT ( noConnection() ) );
    connect ( m_port,SIGNAL ( SMSSend() ),this,SLOT ( SMSSend() ) );
    connect ( m_port,SIGNAL ( doneGetAll() ),this,SLOT ( SyncComplete() ) );
    connect ( m_port,SIGNAL ( newSMSMessage ( SMSMessage ) ),this,SLOT ( newMessage(SMSMessage) ) );

    connect ( m_smslist,SIGNAL ( newContact ( QString,long ) ),m_contactlist,SLOT ( updateContacts ( QString,long ) ) );

    connect ( m_timer, SIGNAL ( timeout() ), this, SLOT ( SyncSms() ) );

    if ( m_xmlhandler->load ( m_saveLocation ) )
    {
        m_smslist->filter ( m_contactlist->getFirstAddress() );
    }

    settingsChanged();
}

KDroid::~KDroid()
{
    if (m_gui) {
        delete m_gui;
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
    if (!args->isSet("send")) {
        if (args->isSet("address")) {
            m_gui->getSendView()->setAddress(args->getOption("address"));
        }
        if (args->isSet("body")) {
            m_gui->getSendView()->setBody(args->getOption("body"));
        }
    }
    if (args->isSet("send") && args->isSet("address") && args->isSet("body")) {
        SMSMessage message;
        message.Body=args->getOption("body");
        message.Address=args->getOption("address");
        message.Time=QDateTime::currentMSecsSinceEpoch();
        sendSMS(message);
    }
}

void KDroid::newMessage(SMSMessage message)
{
    if (message.Type=="Incoming") {
        showNotification ( "KDroid",i18n ( "New Message" ),"newMessage" );
        qDebug() <<"new Message";
    }
}



void KDroid::sendSMS ( SMSMessage message )
{
    Packet packet = Packet ( message );
    m_port->send ( packet );
}

void KDroid::SMSSend()
{
    showNotification ( "KDroid",i18n ( "SMS send" ),"sendMessage" );
    qDebug() <<"Message Send";
}

void KDroid::SyncComplete()
{
    qDebug() <<"Sync Complete";
    connect ( m_port,SIGNAL ( newSMSMessage ( SMSMessage ) ),this,SLOT ( newMessage(SMSMessage) ) );
    showNotification ( "KDroid",i18n ( "Sync complete" ),"syncComplete" );
    m_smslist->filter ( m_contactlist->getFirstAddress() );

    m_xmlhandler->save ( m_saveLocation );

}

void KDroid::SyncSms()
{
    qDebug() <<"Sync start";
    disconnect ( m_port,SIGNAL ( newSMSMessage ( SMSMessage ) ),this,SLOT ( newMessage(SMSMessage) ) );
    if ( Settings::auto_sync() && !m_timer->isActive() )
    {
        m_timer->setInterval ( Settings::timer_interval() *60000 );
        m_timer->start();
    }
    Packet packet =  Packet ( QString ( "Request" ) );
    packet.addArgument ( "getAll" );
    m_port->send ( packet );
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
    if ( m_timer->isActive() )
    {
        m_timer->stop();
    }
    showNotification ( "KDroid",i18n ( "Device unreachable" ),"noConnection" );
}



void KDroid::showNotification ( QString title, QString message, QString type )
{
    m_notify = new KNotification ( type );
    m_notify->setTitle ( title );
    m_notify->setText ( message );
    m_notify->setPixmap ( SmallIcon ( "pda", KIconLoader::SizeMedium ) );
    m_notify->sendEvent();

}

void KDroid::settingsChanged()
{
    qDebug() <<"Settings Changed";
    m_port->setPort ( Settings::port() );
    m_port->setIp ( Settings::ip_address() );
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

Port* KDroid::getPort()
{
    return m_port;
}

#include "kdroid.moc"



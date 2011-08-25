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

#include "kdroidxmlgui.h"
#include "kdroidview.h"
#include "settings.h"

#include <QStringList>
#include <QList>
#include <QDebug>
#include <QDateTime>
#include <QTimer>
#include <QFile>
#include <QDir>

#include <KDE/KApplication>
#include <KConfigDialog>
#include <KStatusBar>
#include <KAction>
#include <KActionCollection>
#include <KStandardAction>
#include <KNotification>
#include <KStandardDirs>
#include <KMessageBox>
#include <KSystemTrayIcon>
#include <KFileDialog>
#include <KLocale>
#include <KUser>
#include <KIconLoader>

KDroid::KDroid()
        : KXmlGuiWindow(),
        m_view ( new KDroidView ( this ) ),
        m_sendview ( new SendView ( this ) ),
        m_timer ( new QTimer() ),
        m_port ( new Port ( this ) ),
        m_smslist ( new SMSList ( this ) ),
        m_contactlist ( new ContactList ( this ) ),
        m_xmlhandler ( new XMLHandler ( m_contactlist,m_smslist,this ) )
{
    setAcceptDrops ( true );

    setCentralWidget ( m_view );

    addDockWidget ( Qt::BottomDockWidgetArea,m_sendview );

    setupActions();

    statusBar()->show();

    setupGUI();
    m_view->setContactModel ( m_contactlist );
    m_view->setSMSModel ( m_smslist );
    QPixmap icon = SmallIcon ( "pda", KIconLoader::SizeMedium );
    m_trayIcon= new KSystemTrayIcon ( icon,this );
    m_trayIcon->show();

    m_saveLocation = KStandardDirs().saveLocation ( "data",qAppName() ).append ( "kdroidData.xml" );

    connect ( m_port,SIGNAL ( newSMSMessage ( SMSMessage ) ),m_smslist,SLOT ( addSMS ( SMSMessage ) ) );
    connect ( m_port,SIGNAL ( newContact ( Contact ) ),m_contactlist,SLOT ( addContact ( Contact ) ) );
    connect ( m_port,SIGNAL ( ackGetAll() ),this,SLOT ( ackGetAll() ) );
    connect ( m_port,SIGNAL ( connectionError() ),this,SLOT ( noConnection() ) );
    connect ( m_port,SIGNAL ( SMSSend() ),this,SLOT ( SMSSend() ) );
    connect ( m_port,SIGNAL ( doneGetAll() ),this,SLOT ( SyncComplete() ) );

    connect ( m_smslist,SIGNAL ( newContactTime ( int,long ) ),m_contactlist,SLOT ( updateContactTime ( int,long ) ) );

    connect ( m_timer, SIGNAL ( timeout() ), this, SLOT ( SyncSms() ) );

    connect ( m_view,SIGNAL ( contactAktivated ( QModelIndex ) ),this,SLOT ( selectionChanged ( QModelIndex ) ) );

    connect ( m_sendview,SIGNAL ( sendSMS ( SMSMessage ) ),this,SLOT ( sendSMS ( SMSMessage ) ) );

    settingsChanged();

    if ( m_xmlhandler->load ( m_saveLocation ) )
    {
        m_smslist->filter ( m_contactlist->getFirstThreadId() );
    }
}

KDroid::~KDroid()
{
    delete m_timer;
}

void KDroid::setupActions()
{
    KStandardAction::quit ( qApp, SLOT ( quit() ), actionCollection() );

    KStandardAction::preferences ( this, SLOT ( optionsPreferences() ), actionCollection() );

    sync = new KAction ( KIcon ( "pda" ), i18n ( "Synchronise" ), this );
    actionCollection()->addAction ( QLatin1String ( "sync_action" ), sync );
    connect ( sync, SIGNAL ( triggered ( bool ) ), this, SLOT ( SyncSms() ) );

    KAction *xmlexport = new KAction ( i18n ( "XML" ), this );
    actionCollection()->addAction ( QLatin1String ( "xml_export" ), xmlexport );
    connect ( xmlexport, SIGNAL ( triggered ( bool ) ), this, SLOT ( xmlExport() ) );

    actionCollection()->addAction ( QLatin1String ( "showsendview" ), m_sendview->toggleViewAction() );


}

void KDroid::xmlExport()
{
    QString file = KFileDialog::getSaveFileName ( KUrl(), i18n ( "*.xml|KDroid file (*.xml)" ),this );
    if ( !file.isEmpty() )
    {
        m_xmlhandler->save ( file );
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

    showNotification ( "KDroid",i18n ( "Sync complete" ),"syncComplete" );
    sync->setEnabled ( true );
    m_smslist->filter ( m_contactlist->getFirstThreadId() );

    m_xmlhandler->save ( m_saveLocation );

}

void KDroid::SyncSms()
{
    //sync->setEnabled ( false );
    qDebug() <<"Sync start";
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
    sync->setEnabled ( true );
    showNotification ( "KDroid",i18n ( "Device unreachable" ),"noConnection" );
    //KMessageBox::error(this,i18n("Phone unreachable"));
}



void KDroid::showNotification ( QString title, QString message, QString type )
{
    m_notify = new KNotification ( type );
    m_notify->setTitle ( title );
    m_notify->setText ( message );
    m_notify->setPixmap ( SmallIcon ( "pda", KIconLoader::SizeMedium ) );
    m_notify->sendEvent();

}

void KDroid::optionsPreferences()
{
    if ( KConfigDialog::showDialog ( "settings" ) )
    {
        return;
    }
    KConfigDialog *dialog = new KConfigDialog ( this, "settings", Settings::self() );
    QWidget *generalSettingsDlg = new QWidget;
    ui_prefs_base.setupUi ( generalSettingsDlg );
    dialog->addPage ( generalSettingsDlg, i18n ( "General" ), "package_setting" );
    connect ( dialog, SIGNAL ( settingsChanged ( QString ) ), this, SLOT ( settingsChanged() ) );
    dialog->setAttribute ( Qt::WA_DeleteOnClose );
    dialog->show();
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

void KDroid::selectionChanged ( QModelIndex index )
{
    m_sendview->setAddress ( index.data ( ContactList::Address ).toString() );
    m_smslist->filter ( index.data ( ContactList::ThreadId ).toInt() );
}


void KDroid::closeEvent ( QCloseEvent *event )
{
    if ( event->spontaneous() && !kapp->sessionSaving() )
    {
        hide();
        event->ignore();
        return;
    }

    event->accept();
    kapp->quit();
}



#include "kdroidxmlgui.moc"


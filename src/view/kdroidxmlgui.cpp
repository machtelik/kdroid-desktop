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
#include <QCloseEvent>

#include <KUniqueApplication>
#include <KConfigDialog>
#include <KStatusBar>
#include <KAction>
#include <KActionCollection>
#include <KStandardAction>
#include <KSystemTrayIcon>
#include <KFileDialog>
#include <KLocale>
#include <KUser>
#include <KIconLoader>
#include <KNotifyConfigWidget>

KDroidXmlGui::KDroidXmlGui(KDroid *app)
        : KXmlGuiWindow(),
        m_view ( new KDroidView ( this ) ),
        m_sendview ( new SendView ( this ) ),
        m_app(app)
{
    setAcceptDrops ( true );

    setCentralWidget ( m_view );

    addDockWidget ( Qt::BottomDockWidgetArea,m_sendview );

    setupActions();

    statusBar()->show();

    setupGUI();

    m_view->setContactModel (m_app->getContactList() );
    m_view->setSMSModel ( m_app->getSMSList() );

    connect ( m_view,SIGNAL ( contactAktivated ( QModelIndex ) ),this,SLOT ( selectionChanged ( QModelIndex ) ) );

    connect ( m_sendview,SIGNAL ( sendSMS ( QString, QString ) ),m_app,SLOT ( sendSMS ( QString, QString ) ) );

}

KDroidXmlGui::~KDroidXmlGui()
{
}

void KDroidXmlGui::setupActions()
{
    KStandardAction::quit ( qApp, SLOT ( quit() ), actionCollection() );

    KStandardAction::preferences ( this, SLOT ( optionsPreferences() ), actionCollection() );

    sync = new KAction ( KIcon ( "pda" ), i18n ( "Synchronise" ), this );
    actionCollection()->addAction ( QLatin1String ( "sync_action" ), sync );
    connect ( sync, SIGNAL ( triggered ( bool ) ), m_app, SLOT ( SyncSms() ) );

    KAction *xmlexport = new KAction ( KIcon ( "document-save-as" ), i18n ( "..XML" ), this );
    actionCollection()->addAction ( QLatin1String ( "xml_export" ), xmlexport );
    connect ( xmlexport, SIGNAL ( triggered ( bool ) ), this, SLOT ( xmlExport() ) );

    QAction *showSendView = m_sendview->toggleViewAction();
    showSendView->setIcon( KIcon ( "mail-message-new" ) );
    actionCollection()->addAction ( QLatin1String ( "showsendview" ), showSendView );


}

void KDroidXmlGui::xmlExport()
{
    QString file = KFileDialog::getSaveFileName ( KUrl(), i18n ( "*.xml|KDroid file (*.xml)" ),this );
    if ( !file.isEmpty() )
    {
        m_app->getXMLHandler()->save(file);
    }
}

void KDroidXmlGui::optionsPreferences()
{
    if ( KConfigDialog::showDialog ( "settings" ) )
    {
        return;
    }
    KConfigDialog *dialog = new KConfigDialog ( this, "settings", Settings::self() );
    QWidget *generalSettingsDlg = new QWidget;
    ui_prefs_base.setupUi ( generalSettingsDlg );
    KNotifyConfigWidget *notifyDialog = new KNotifyConfigWidget(dialog);
    notifyDialog->setApplication("kdroid");
    dialog->addPage ( generalSettingsDlg, i18n ( "General" ), "preferences-other" );
    dialog->addPage ( notifyDialog, i18n ( "Notifications" ), "dialog-information" );
    dialog->setAttribute ( Qt::WA_DeleteOnClose );
    connect(dialog,SIGNAL(applyClicked()),notifyDialog,SLOT(save()));
    connect(dialog,SIGNAL(okClicked()),notifyDialog,SLOT(save()));
    connect(notifyDialog,SIGNAL(changed(bool)) , dialog , SLOT(enableButtonApply(bool)));
    connect ( dialog, SIGNAL ( settingsChanged ( QString ) ), m_app, SLOT ( settingsChanged() ) );
    dialog->show();
}

void KDroidXmlGui::selectionChanged ( QModelIndex index )
{
    m_sendview->setAddress ( index.data ( ContactList::Address ).toString() );
    m_app->getSMSList()->filter ( index.data ( ContactList::Address ).toString() );
}


void KDroidXmlGui::closeEvent ( QCloseEvent *event )
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

SendView* KDroidXmlGui::getSendView()
{
    return m_sendview;
}

KDroidView* KDroidXmlGui::getMainView()
{
    return m_view;
}

void KDroidXmlGui::setEnableSyncButton(bool b)
{
    sync->setEnabled(b);
}

#include "kdroidxmlgui.moc"

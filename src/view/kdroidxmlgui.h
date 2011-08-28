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

#ifndef KDROIDXMLGUI_H
#define KDROIDXMLGUI_H


#include <KXmlGuiWindow>
#include <QList>
#include <QTreeWidgetItem>
#include <QTimer>
#include <KNotification>
#include <KSystemTrayIcon>
#include <QModelIndex>
#include <KAction>

#include "../kdroid.h"
#include "ui_prefs_base.h"
#include "sendview.h"
#include "../net/port.h"
#include "../sms/smslist.h"
#include "../contact/contactlist.h"
#include "../xmlhandler.h"

class KDroidView;
class KDroid;

class KDroidXmlGui : public KXmlGuiWindow
{
    Q_OBJECT
public:
    KDroidXmlGui(KDroid *app);
    virtual ~KDroidXmlGui();

    SendView* getSendView();
    KDroidView* getMainView();

private slots:
    void optionsPreferences();
    void closeEvent(QCloseEvent *event);
    void xmlExport();
    void selectionChanged(QModelIndex index);

private:
    void setupActions();

    Ui::prefs_base ui_prefs_base ;
    KDroidView *m_view;
    SendView *m_sendview;
    KSystemTrayIcon *m_trayIcon;

    KAction *sync;

    KDroid *m_app;
};

#endif // _KDROIDXMLGUI_H_

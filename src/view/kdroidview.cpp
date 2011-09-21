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

#include "kdroidview.h"

#include "settings.h"

#include <KLocale>

#include <QLabel>
#include <QString>
#include <QPlainTextEdit>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QStringList>
#include <QDebug>
#include <QStandardItemModel>

#include "contactitemdelegate.h"
#include "smsitemdelegate.h"

KDroidView::KDroidView(QWidget *)
{
    ui_kdroidview_base.setupUi(this);
    ui_kdroidview_base.ContactListView->setItemDelegate(new ContactItemDelegate(this));
    ui_kdroidview_base.SmsListView->setItemDelegate(new SMSItemDelegate(this));
    ui_kdroidview_base.SmsListView->setResizeMode(QListView::Adjust);
    connect(ui_kdroidview_base.ContactListView,SIGNAL(entered(QModelIndex)),this,SIGNAL(contactAktivated(QModelIndex)));
    connect(ui_kdroidview_base.ContactListView,SIGNAL(clicked(QModelIndex)),this,SIGNAL(contactAktivated(QModelIndex)));
    connect(ui_kdroidview_base.ContactFilterLineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(filterChanged(QString)));
}

void KDroidView::setContactModel(ContactList *contacts)
{
    ui_kdroidview_base.ContactListView->setModel(contacts);
    m_contactsModel = contacts;
}

void KDroidView::setSMSModel(SMSList* sms)
{
    ui_kdroidview_base.SmsListView->setModel(sms);
}

KDroidView::~KDroidView()
{

}

QListView* KDroidView::getContactListView()
{
    return ui_kdroidview_base.ContactListView;
}

QListView* KDroidView::getSmsListView()
{
    return ui_kdroidview_base.SmsListView;
}


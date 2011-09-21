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

#ifndef KDROIDVIEW_H
#define KDROIDVIEW_H

#include <QWidget>
#include <QStandardItem>

#include "ui_kdroidview_base.h"

#include "../contact/contactlist.h"
#include "../sms/smslist.h"

class QPainter;
class KUrl;

class KDroidView : public QWidget
{
    Q_OBJECT
public:
    KDroidView(QWidget *parent);
    virtual ~KDroidView();
    void setContactModel(ContactList *contacts);
    void setSMSModel(SMSList *sms);

    QListView* getSmsListView();
    QListView* getContactListView();
private:
    Ui::kdroidview_base ui_kdroidview_base;
    ContactList *m_contactsModel;
private slots:
signals:
    void contactAktivated(QModelIndex index);
    void filterChanged(QString filter);
public slots:
};

#endif // KDroidVIEW_H

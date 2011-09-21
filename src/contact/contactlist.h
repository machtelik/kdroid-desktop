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

#ifndef CONTACTHANDLER_H
#define CONTACTHANDLER_H

#include <QAbstractListModel>

#include "contact.h"

class ContactList : public QAbstractListModel
{
    Q_OBJECT
public:
    ContactList(QObject * parent = 0);
    virtual ~ContactList();
    virtual int rowCount(const QModelIndex &index) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    QString getFirstAddress();
    int size();
    Contact getAt(int at);
    enum ContactData {ID=0,Name=25,Address=26};
public slots:
    void addContact(Contact contact);
    void clear();
    void filter(QString filter);
    void updateContacts(QString address, long time);
signals:
private:
    void clearView();
    QList<Contact> *m_contactlist;
    QList<const Contact*> *m_filteredcontactlist;
    void sortedInsert(const Contact* contact);
    QString m_filter;
};

#endif // CONTACTHANDLER_H

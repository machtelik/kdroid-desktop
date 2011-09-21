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

#include "contactlist.h"

#include <QDebug>

#include <KLocale>

#include "../util/phonenumberutil.h"

ContactList::ContactList ( QObject * parent ) :
        QAbstractListModel ( parent ),
        m_contactlist ( new QList<Contact>() ),
        m_filteredcontactlist(new QList<const Contact*>())
{

}

ContactList::~ContactList()
{
    delete m_contactlist;
    delete m_filteredcontactlist;
}

void ContactList::addContact ( Contact contact )
{
    m_contactlist->append(contact);
    if (contact.Name.contains(m_filter,Qt::CaseInsensitive) || contact.Address.contains(m_filter,Qt::CaseInsensitive)) {
        sortedInsert ( &m_contactlist->last() );
    }
}

int ContactList::size()
{
    return m_contactlist->size();
}

Contact ContactList::getAt ( int at )
{
    return m_contactlist->at ( at );
}

void ContactList::filter(QString filter)
{
    m_filter=filter;
    clearView();
    for (int i = 0; i<m_contactlist->size();++i) {
        if (m_contactlist->at(i).Name.contains(filter,Qt::CaseInsensitive) || m_contactlist->at(i).Address.contains(filter,Qt::CaseInsensitive)) {
            sortedInsert( &m_contactlist->at(i) );
        }
    }
}

void ContactList::sortedInsert (const Contact* contact )
{
    Contact contactTmp = *contact;
    for ( int i = 0;i<m_filteredcontactlist->size();++i )
    {
        Contact c = *m_filteredcontactlist->at ( i );
        if ( contactTmp<c )
        {
            beginInsertRows ( QModelIndex(),i,i );
            m_filteredcontactlist->insert ( i,contact );
            endInsertRows();
            return;
        }
    }
    beginInsertRows ( QModelIndex(),m_filteredcontactlist->size(),m_filteredcontactlist->size() );
    m_filteredcontactlist->append ( contact );
    endInsertRows();
}

QString ContactList::getFirstAddress()
{
    if ( !m_filteredcontactlist->isEmpty() )
    {
        return m_filteredcontactlist->first()->Address;
    }
    return "";
}


void ContactList::updateContacts (QString address , long time )
{
    bool found = false;
    for ( int i = 0;i<m_contactlist->size();++i )
    {
        if ( PhoneNumberUtil::compare(m_contactlist->at ( i ).Address,address) )
        {
            found=true;
            if (m_contactlist->at ( i ).lastContactTime<time) {
                beginRemoveRows ( QModelIndex(),i,i );
                Contact c = m_contactlist->takeAt ( i );
                endRemoveRows();
                c.lastContactTime=time;
                m_contactlist->append( c );
                filter(m_filter);
                return;
            }
        }
    }
    if (!found) {
        Contact contact;
        contact.Name=i18n("Unknown");
        contact.Address=address;
        contact.lastContactTime=time;
        m_contactlist->append(contact);
    }
    filter(m_filter);
}


void ContactList::clear()
{
    clearView();
    m_contactlist->clear();
}

void ContactList::clearView()
{
    beginRemoveRows ( QModelIndex(),0,m_filteredcontactlist->size()-1 );
    m_filteredcontactlist->clear();
    endRemoveRows();
}


int ContactList::rowCount ( const QModelIndex& index ) const
{
    Q_UNUSED ( index );
    return m_filteredcontactlist->size();
}

QVariant ContactList::data ( const QModelIndex& index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    if ( index.row() >= m_filteredcontactlist->size() )
        return QVariant("-1");

    if ( role == Name )
        return QVariant ( m_filteredcontactlist->at ( index.row() )->Name );
    else if ( role == Address )
        return QVariant ( m_filteredcontactlist->at ( index.row() )->Address );
    else if ( role == ID )
        return QVariant ( m_filteredcontactlist->at ( index.row() )->Id );
    else
        return QVariant();
}


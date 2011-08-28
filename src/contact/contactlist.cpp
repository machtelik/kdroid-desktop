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

#include "../util/phonenumberutil.h"

#include "KLocale"

ContactList::ContactList ( QObject * parent ) :
        QAbstractListModel ( parent ),
        m_contactlist ( new QList<Contact>() )
{

}

ContactList::~ContactList()
{
    delete m_contactlist;
}

void ContactList::addContact ( Contact contact )
{
    sortedInsert ( contact );
}

int ContactList::size()
{
    return m_contactlist->size();
}

Contact ContactList::getAt ( int at )
{
    return m_contactlist->at ( at );
}



void ContactList::sortedInsert ( Contact& contact )
{
    for ( int i = 0;i<m_contactlist->size();++i )
    {
        Contact c = m_contactlist->at ( i );
        if ( contact<c )
        {
            beginInsertRows ( QModelIndex(),i,i );
            m_contactlist->insert ( i,contact );
            endInsertRows();
            return;
        }
    }
    beginInsertRows ( QModelIndex(),m_contactlist->size(),m_contactlist->size() );
    m_contactlist->append ( contact );
    endInsertRows();
}

QString ContactList::getFirstAddress()
{
    if ( !m_contactlist->isEmpty() )
    {
        return m_contactlist->first().Address;
    }
    return "-1";
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
                sortedInsert ( c );
                return;
            }
        }
    }
    if (!found) {
        Contact contact;
        contact.Name=i18n("Unknown");
        contact.Address=address;
        contact.lastContactTime=time;
        sortedInsert(contact);
    }
}


void ContactList::clear()
{
    beginRemoveRows ( QModelIndex(),0,m_contactlist->size()-1 );
    m_contactlist->clear();
    endRemoveRows();
}

int ContactList::rowCount ( const QModelIndex& index ) const
{
    Q_UNUSED ( index );
    return m_contactlist->size();
}

QVariant ContactList::data ( const QModelIndex& index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    if ( index.row() >= m_contactlist->size() )
        return QVariant();

    if ( role == Name )
        return QVariant ( m_contactlist->at ( index.row() ).Name );
    else if ( role == Address )
        return QVariant ( m_contactlist->at ( index.row() ).Address );
    else if ( role == ID )
        return QVariant ( m_contactlist->at ( index.row() ).Id );
    else
        return QVariant();
}

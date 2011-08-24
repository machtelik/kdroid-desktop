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


#include "smslist.h"

#include <QDateTime>
#include <QDebug>

#include <KLocale>

SMSList::SMSList ( QObject * parent ) :
        QAbstractListModel ( parent ),
        m_smslist ( new QList<SMSMessage>() ),
        m_filteredlist ( new QList<const SMSMessage*>() ),
        m_filter ( -2 )
{

}

SMSList::~SMSList()
{
    delete m_smslist;
    delete m_filteredlist;
}


void SMSList::addSMS ( SMSMessage message )
{

    m_smslist->append ( message );
    emit newContactTime ( message.ThreadId,message.Time );
    if ( message.ThreadId==m_filter )
    {
        sortedInsert ( &m_smslist->last() );
    }
}

int SMSList::size()
{
    return m_smslist->size();
}

SMSMessage SMSList::getAt ( int at )
{
    return m_smslist->at ( at );
}



void SMSList::filter ( int filter )
{
    m_filter=filter;
    beginRemoveRows ( QModelIndex(),0,m_filteredlist->size()-1 );
    m_filteredlist->clear();
    endRemoveRows();

    for ( int i = 0;i<m_smslist->size();++i )
    {
        if ( m_smslist->at ( i ).ThreadId==m_filter )
        {
            sortedInsert ( &m_smslist->at ( i ) );
        }
    }
}

void SMSList::sortedInsert ( const SMSMessage* message )
{
    SMSMessage me = *message;
    for ( int i = 0;i<m_filteredlist->size();++i )
    {
        SMSMessage m = *m_filteredlist->at ( i );
        if ( m<me )
        {
            beginInsertRows ( QModelIndex(),i,i );
            m_filteredlist->insert ( i,message );
            endInsertRows();
            return;
        }
    }
    beginInsertRows ( QModelIndex(),m_filteredlist->size(),m_filteredlist->size() );
    m_filteredlist->append ( message );
    endInsertRows();
}


void SMSList::clear()
{
    beginRemoveRows ( QModelIndex(),0,m_filteredlist->size()-1 );
    m_smslist->clear();
    m_filteredlist->clear();
    m_filter=-2;
    endRemoveRows();
}

int SMSList::rowCount ( const QModelIndex& index ) const
{
    Q_UNUSED ( index );
    return m_filteredlist->size();
}

QVariant SMSList::data ( const QModelIndex& index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    if ( index.row() >= m_filteredlist->size() )
        return QVariant();

    if ( role == Body )
        return QVariant ( m_filteredlist->at ( index.row() )->Body );
    else if ( role == Type )
        return QVariant ( m_filteredlist->at ( index.row() )->Type );
    else if ( role == Id )
        return QVariant ( m_filteredlist->at ( index.row() )->Id );
    else if ( role == Time )
        return QVariant ( m_filteredlist->at ( index.row() )->Time );
    else if ( role == ContactId )
        return QVariant ( m_filteredlist->at ( index.row() )->PersonId );
    else if ( role == Address )
        return QVariant ( m_filteredlist->at ( index.row() )->Address );
    else if ( role == ThreadId )
        return QVariant ( m_filteredlist->at ( index.row() )->ThreadId );
    else
        return QVariant();
}





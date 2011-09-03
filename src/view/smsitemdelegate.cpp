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

#include "smsitemdelegate.h"

#include "../sms/smslist.h"

#include <QApplication>
#include <QPainter>
#include <QDateTime>

#include <KGlobalSettings>
#include <KGlobal>
#include <KLocale>

const int SPACING = 4;
const int MAXCHARS = 80;

SMSItemDelegate::SMSItemDelegate ( QObject * parent ) :
        QStyledItemDelegate ( parent )
{
    m_palette = new QPalette ( QApplication::palette() );
}

SMSItemDelegate::~SMSItemDelegate()
{
    delete m_palette;
}

QStringList SMSItemDelegate::wordWrap ( const QString& str ) const
{

    QString tempStr= str;
    tempStr.replace ( "\n"," " );
    int len = tempStr.length(), pos= MAXCHARS;

    while ( pos < len-1 )
    {

        int tempPos=pos;
        while ( tempStr.at ( tempPos ) !=' ' && tempPos > 0 )
        {

            --tempPos;
        }
        if ( tempPos > 0 ) pos=tempPos;

        tempStr.replace ( pos,1,'\n' );
        pos+=pos;
    }

    return tempStr.split ( "\n" );
}

void SMSItemDelegate::paint ( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
    QStyleOptionViewItemV4 optV4 = option;
    initStyleOption ( &optV4, index );

    painter->save();

    painter->setRenderHints ( QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing );
    painter->setClipRect ( optV4.rect );

    QStyle *style = QApplication::style();
    style->drawPrimitive ( QStyle::PE_PanelItemViewItem, &option, painter );

    QFont font = KGlobalSettings::generalFont();

    const QFontMetrics fontMetrics ( font );

    painter->setFont ( font );


    //TODO: Make word wrapping dynamic
    QString body = index.data ( SMSList::Body ).toString();
    QStringList list = wordWrap ( body );
    QPen bodyPen = painter->pen();
    bodyPen.setColor ( m_palette->color ( QPalette::Normal, QPalette::Text ) );
    painter->setPen ( bodyPen );
    QRect bodyRect = optV4.rect;
    bodyRect.setX ( SPACING * 2 );
    bodyRect.setWidth ( fontMetrics.width ( body,body.length() ) );
    for ( int i=0;i<list.size();++i )
    {
        bodyRect.setY ( SPACING*2 + optV4.rect.y() +fontMetrics.height() *i );
        painter->drawText ( bodyRect,fontMetrics.elidedText ( list.at ( i ), Qt::ElideRight, bodyRect.width() ) );
    }


    QPen typePen = painter->pen();
    typePen.setColor ( m_palette->color ( QPalette::Disabled, QPalette::Text ) );
    painter->setPen ( typePen );
    QRect typeRect = optV4.rect;
    typeRect.setX ( SPACING * 2 );
    typeRect.setY ( optV4.rect.y() +SPACING*3 +fontMetrics.height() *list.size() );
    QString type;
    if ( index.data ( SMSList::Type ).toString() ==QString ( "Incoming" ) )
    {
        type = i18n ( "Incoming" );
    }
    else if ( index.data ( SMSList::Type ).toString() ==QString ( "Outgoing" ) )
    {
        type = i18n ( "Outgoing" );
    }
    typeRect.setWidth ( fontMetrics.width ( type,type.length() ) +SPACING );
    painter->drawText ( typeRect,fontMetrics.elidedText ( type, Qt::ElideRight, typeRect.width() ) );


    QDateTime dt;
    dt.setMSecsSinceEpoch ( index.data ( SMSList::Time ).toLongLong() );
    QString dateString = KGlobal::locale()->formatDateTime ( dt,KLocale::FancyLongDate,true );
    QRect dateRect = optV4.rect;
    dateRect.setX ( typeRect.width() +  SPACING * 3 );
    dateRect.setY ( optV4.rect.y() +SPACING*3 +fontMetrics.height() *list.size() );
    dateRect.setWidth ( fontMetrics.width ( dateString,dateString.length() ) +SPACING );
    painter->drawText ( dateRect,fontMetrics.elidedText ( dateString, Qt::ElideRight, dateRect.width() ) );


    painter->restore();
}

QSize SMSItemDelegate::sizeHint ( const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
    Q_UNUSED ( option );
    Q_UNUSED ( index );

    QFont font = KGlobalSettings::generalFont();
    const QFontMetrics fontMetrics ( font );

    QString body = index.data ( SMSList::Body ).toString();
    QStringList list = wordWrap ( body );

    return QSize ( 0,fontMetrics.height() *list.size() +SPACING*9 );
}


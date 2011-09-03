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

#include "contactitemdelegate.h"

#include "../contact/contactlist.h"

#include <QPainter>
#include <QApplication>

#include <KGlobalSettings>
#include <KIconLoader>
#include <KLocale>

const int SPACING = 4;

ContactItemDelegate::ContactItemDelegate ( QObject * parent ) :
        QStyledItemDelegate ( parent )
{
    m_palette = new QPalette ( QApplication::palette() );
}

ContactItemDelegate::~ContactItemDelegate()
{
    delete m_palette;
}

void ContactItemDelegate::paint ( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
    QStyleOptionViewItemV4 optV4 = option;
    initStyleOption ( &optV4, index );

    painter->save();

    painter->setRenderHints ( QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing );
    painter->setClipRect ( optV4.rect );

    QFont font = KGlobalSettings::generalFont();

    const QFontMetrics fontMetrics ( font );

    painter->setFont ( font );

    QStyle *style = QApplication::style();
    style->drawPrimitive ( QStyle::PE_PanelItemViewItem, &option, painter );

    QRect iconRect = optV4.rect;
    iconRect.setSize ( QSize ( fontMetrics.height() *2+SPACING*2, fontMetrics.height() *2+SPACING*2 ) );
    iconRect.moveTo ( QPoint ( iconRect.x() + SPACING, iconRect.y() + SPACING ) );

    QPixmap avatar;

    bool noContactAvatar = avatar.isNull();

    if ( noContactAvatar )
    {
        avatar = SmallIcon ( "im-user", KIconLoader::SizeMedium );
    }

    painter->drawPixmap ( iconRect, avatar );

    QPen namePen = painter->pen();
    namePen.setColor ( m_palette->color ( QPalette::Normal, QPalette::Text ) );

    painter->setPen ( namePen );

    QRect userNameRect = optV4.rect;
    userNameRect.setX ( iconRect.x() + iconRect.width() + SPACING * 2 );
    userNameRect.setY ( userNameRect.y() + SPACING*2 );
    userNameRect.setWidth ( userNameRect.width() - SPACING );

    painter->drawText ( userNameRect,fontMetrics.elidedText ( index.data ( ContactList::Name ).toString(), Qt::ElideRight, userNameRect.width() ) );

    QRect addressRect = optV4.rect;
    addressRect.setX ( iconRect.x() + iconRect.width() + SPACING * 2 );
    addressRect.setWidth ( optV4.rect.width() - addressRect.x() - SPACING );
    addressRect.setY ( addressRect.y() + fontMetrics.height() +SPACING*2 );

    QPen addressPen = painter->pen();
    addressPen.setColor ( m_palette->color ( QPalette::Disabled, QPalette::Text ) );

    painter->setPen ( addressPen );

    painter->drawText ( addressRect,fontMetrics.elidedText ( i18n ( "Number: " ) +index.data ( ContactList::Address ).toString(),
                        Qt::ElideRight, addressRect.width() ) );
    painter->restore();
}

QSize ContactItemDelegate::sizeHint ( const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
    Q_UNUSED ( option );
    Q_UNUSED ( index );

    QFont font = KGlobalSettings::generalFont();
    const QFontMetrics fontMetrics ( font );
    return QSize ( 0, fontMetrics.height() *2+SPACING*4 );
}

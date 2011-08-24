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


#include "sendview.h"


#include <QtGui/QWidget>
#include <KMessageBox>
#include <QDateTime>
#include <QLabel>
#include <qtextedit.h>

SendView::SendView(QWidget *) :
        m_view(new QWidget(this))
{
    ui_sendview_base.setupUi(m_view);
    setWidget(m_view);
    setObjectName("sendView");
    setWindowTitle(i18n("Send Sms"));
    connect(ui_sendview_base.SendButton, SIGNAL(pressed()), this, SLOT(Send()));
    connect(ui_sendview_base.SmsTextEdit,SIGNAL(textChanged()),this,SLOT(textChanged()));
    //TODO: QCompleter for address field
}

void SendView::textChanged()
{
  ui_sendview_base.textLength->setText(QString::number(ui_sendview_base.SmsTextEdit->toPlainText().size()));
}



void SendView::Send() {
    QString text = ui_sendview_base.SmsTextEdit->toPlainText();
    QString address = ui_sendview_base.AddressEdit->text();
    if (text!="" && address!="") {
        SMSMessage message;
        message.Body=text;
        message.Address=address;
	message.Time=QDateTime::currentMSecsSinceEpoch();
        emit sendSMS(message);
        ui_sendview_base.SmsTextEdit->clear();
    }
}

void SendView::setAddress(QString address)
{
    ui_sendview_base.AddressEdit->setText(address);
}


#include "view/sendview.moc"

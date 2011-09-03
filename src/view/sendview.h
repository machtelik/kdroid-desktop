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


#ifndef SENDVIEW_H
#define SENDVIEW_H

#include <QDockWidget>
#include <QString>

#include "../sms/smsmessage.h"

#include "ui_sendview_base.h"

class SendView: public QDockWidget
{
    Q_OBJECT
public:
    SendView(QWidget *parent);
    void setAddress(QString address);
    void setBody(QString body);
public slots:
    void Send();
private slots:
    void textChanged();
signals:
    void sendSMS(SMSMessage message);
private:
    Ui::sendview_base ui_sendview_base;
    QWidget *m_view;


};

#endif // SENDVIEW_H

/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Mike Achtelik <mike.achtelik@googlemail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#ifndef XMLHANDLER_H
#define XMLHANDLER_H

#include <QObject>
#include <QXmlStreamWriter>

#include <KSaveFile>

#include "sms/smslist.h"
#include "contact/contactlist.h"

class XMLHandler : public QObject
{
    Q_OBJECT
public:
    XMLHandler(ContactList *contactlist,SMSList *smslist, QObject * parent = 0);
    ~XMLHandler();
public slots:
    void save(QString filename);
    bool load( QString filename);
private:
    void writeContacts(QXmlStreamWriter &writer);
    void writeSms(QXmlStreamWriter &writer);
    ContactList *m_contactlist;
    SMSList *m_smslist;
    SMSMessage parseSMS(QXmlStreamReader &reader);
    Contact parseContact(QXmlStreamReader &reader);
    QString parseString(QXmlStreamReader &reader);
};

#endif // XMLEXPORT_H

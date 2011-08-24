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


#include "xmlhandler.h"

#include <KLocale>

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDebug>


XMLHandler::XMLHandler ( ContactList *contactlist,SMSList *smslist, QObject * parent ) :
        QObject ( parent ),
        m_contactlist ( contactlist ),
        m_smslist ( smslist )
{
}

XMLHandler::~XMLHandler()
{
}

void XMLHandler::save ( QString filename )
{
    KSaveFile file ( filename );
    file.open();
    QXmlStreamWriter  writer ( &file );
    writer.setAutoFormatting ( true );
    writer.writeStartDocument();
    writer.writeStartElement ( "KDroid" );
    writeContacts ( writer );
    writeSms ( writer );
    writer.writeEndElement();
    writer.writeEndDocument();

    file.finalize();
    file.close();
    qDebug() <<"Data saved";
}

void XMLHandler::writeContacts ( QXmlStreamWriter& writer )
{
    writer.writeStartElement ( "Contacts" );
    for ( int i = 0;i<m_contactlist->size();++i )
    {
        Contact contact = m_contactlist->getAt ( i );
        writer.writeStartElement ( "Contact" );
        writer.writeTextElement ( "Id",QString::number ( contact.Id ) );
        writer.writeTextElement ( "Address",contact.Address );
        writer.writeTextElement ( "Name",contact.Name );
        writer.writeTextElement ( "lastContactTime",QString::number ( contact.lastContactTime ) );
        writer.writeTextElement ( "ThreadId",QString::number ( contact.ThreadId ) );
        writer.writeEndElement();
    }
    writer.writeEndElement();
}

void XMLHandler::writeSms ( QXmlStreamWriter& writer )
{
    writer.writeStartElement ( "Messages" );
    for ( int i = 0;i<m_smslist->size();++i )
    {
        SMSMessage message = m_smslist->getAt ( i );
        writer.writeStartElement ( "Message" );
        writer.writeTextElement ( "Id",QString::number ( message.Id ) );
        writer.writeTextElement ( "ThreadId",QString::number ( message.ThreadId ) );
        writer.writeTextElement ( "ContactId",QString::number ( message.PersonId ) );
        writer.writeTextElement ( "Address",message.Address );
        writer.writeTextElement ( "Body",message.Body );
        writer.writeTextElement ( "Time",QString::number ( message.Time ) );
        writer.writeTextElement ( "Type",message.Type );
        writer.writeEndElement();
    }
    writer.writeEndElement();
}



bool XMLHandler::load ( QString filename )
{
    QFile file ( filename );
    if ( file.open ( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        QXmlStreamReader reader ( &file );
        while ( !reader.atEnd() && !reader.hasError() )
        {
            QXmlStreamReader::TokenType token = reader.readNext();
            if ( token == QXmlStreamReader::StartDocument )
            {
                continue;
            }
            if ( token == QXmlStreamReader::StartElement )
            {
                if ( reader.name() == "Contacts" )
                {
                    continue;
                }
                if ( reader.name() == "Contact" )
                {
                    m_contactlist->addContact ( parseContact ( reader ) );
                }
                if ( reader.name() == "Messages" )
                {
                    continue;
                }
                if ( reader.name() == "Message" )
                {
                    m_smslist->addSMS ( parseSMS ( reader ) );
                }
            }
        }
        file.close();
        qDebug() <<"Data loaded";
        return true;
    }
    return false;
}

Contact XMLHandler::parseContact ( QXmlStreamReader& reader )
{
    Contact contact;
    if ( reader.tokenType() != QXmlStreamReader::StartElement && reader.name() == "Contact" )
    {
        return contact;
    }

    reader.readNext();

    while ( ! ( reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "Contact" ) )
    {
        if ( reader.tokenType() == QXmlStreamReader::StartElement )
        {
            if ( reader.name() == "Id" )
            {
                contact.Id=parseString ( reader ).toInt();
            }
            if ( reader.name() == "Address" )
            {
                contact.Address=parseString ( reader );
            }
            if ( reader.name() == "Name" )
            {
                contact.Name=parseString ( reader );
            }
            if ( reader.name() == "lastContactTime" )
            {
                contact.lastContactTime=parseString ( reader ).toLong();
            }
            if ( reader.name() == "ThreadId" )
            {
                contact.ThreadId=parseString ( reader ).toInt();
            }
        }
        reader.readNext();
    }
    return contact;
}

SMSMessage XMLHandler::parseSMS ( QXmlStreamReader& reader )
{
    SMSMessage message;
    if ( reader.tokenType() != QXmlStreamReader::StartElement && reader.name() == "Message" )
    {
        return message;
    }

    reader.readNext();

    while ( ! ( reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == "Message" ) )
    {
        if ( reader.tokenType() == QXmlStreamReader::StartElement )
        {
            if ( reader.name() == "Id" )
            {
                message.Id=parseString ( reader ).toInt();
            }
            if ( reader.name() == "ThreadId" )
            {
                message.ThreadId=parseString ( reader ).toInt();
            }
            if ( reader.name() == "ContactId" )
            {
                message.PersonId=parseString ( reader ).toInt();
            }
            if ( reader.name() == "Address" )
            {
                message.Address=parseString ( reader );
            }
            if ( reader.name() == "Body" )
            {
                message.Body=parseString ( reader );
            }
            if ( reader.name() == "Type" )
            {
                message.Type=parseString ( reader );
            }
            if ( reader.name() == "Time" )
            {
                message.Time=parseString ( reader ).toLong();
            }
        }
        reader.readNext();
    }
    return message;
}

QString XMLHandler::parseString ( QXmlStreamReader& reader )
{
    reader.readNext();
    if ( reader.tokenType() != QXmlStreamReader::Characters )
    {
        return "";
    }
    return reader.text().toString();
}



#include "xmlhandler.moc"


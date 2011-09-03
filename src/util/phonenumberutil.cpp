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

#include "phonenumberutil.h"

#include <QRegExp>
#include <QDebug>

PhoneNumberUtil::PhoneNumberUtil()
{

}

PhoneNumberUtil::~PhoneNumberUtil()
{

}


bool PhoneNumberUtil::compare(QString a, QString b)
{
    QString normA =  reverse(normalizeNumber(a));
    QString normB =  reverse(normalizeNumber(b));
    int max =normA.size()<normB.size()?normA.size():normB.size();
    int comp = 0;
    for (int i=0;i<max;++i) {
        if (normA.at(i)==normB.at(i)) {
            ++comp;
        } else {
            break;
        }
    }
    if (max!=0) {
        if ((double)comp/max>0.75) { //if 75% of the number are the same, it is the same
            return true;
        }
    }
    return false;
}

QString PhoneNumberUtil::normalizeNumber(QString a)
{
    QString address = a.simplified();

    address.remove(QRegExp(" "));
    address.remove(QRegExp("\\+[0-9][0-9]")); //remove country code
    charToNumber(address);
    address.remove(QRegExp("[^0-9]"));

    return address;
}

QString PhoneNumberUtil::reverse(QString str)
{
    QString newStr;
    for (int i = str.size()-1;i>=0;--i) {
        newStr.append(str.at(i));
    }
    return newStr;
}

QString& PhoneNumberUtil::charToNumber(QString &str)
{
    str.replace(QRegExp("[ABCabc]"),"2");
    str.replace(QRegExp("[DEFdef]"),"3");
    str.replace(QRegExp("[GHIghi]"),"4");
    str.replace(QRegExp("[JKLjkl]"),"5");
    str.replace(QRegExp("[MNOmno]"),"6");
    str.replace(QRegExp("[PQRSpqrs]"),"7");
    str.replace(QRegExp("[TUVtuv]"),"8");
    str.replace(QRegExp("[WXYZwxyz]"),"9");
    return str;
}



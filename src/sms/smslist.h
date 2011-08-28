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


#ifndef SMSHANDLER_H
#define SMSHANDLER_H

#include <QAbstractListModel>

#include "smsmessage.h"

class SMSList : public QAbstractListModel
{
  Q_OBJECT
public:
  SMSList(QObject * parent = 0);
  ~SMSList();
  virtual int rowCount(const QModelIndex &index) const;
  virtual QVariant data(const QModelIndex &index, int role) const;
  int size();
  SMSMessage getAt(int at);
  enum SMSData {Body=0,Address=25,Id=27,Type=29,Time=30};
public slots:
  void addSMS(SMSMessage message);
  void clear();
  void filter(QString filter);
signals:
  void newContactTime(QString address,long time);
private:
  QList<SMSMessage> *m_smslist;
  QList<const SMSMessage*> *m_filteredlist;
  void sortedInsert(const SMSMessage *message);
  QString m_filter;
};

#endif // SMSHANDLER_H

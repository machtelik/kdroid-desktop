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

#ifndef TCPSERVERPORT_H
#define TCPSERVERPORT_H

#include "tcpport.h"

#include <QThread>
#include <QTcpServer>

class TCPServerPort : public TCPPort
{
Q_OBJECT
public:
    TCPServerPort(Dispatcher *dispatcher, QObject* parent = 0);
    virtual ~TCPServerPort();
public slots:
  virtual void setPort(int port);
private slots:
  void connected();
private:
  QTcpServer *m_serversocket;
};

#endif // TCPSERVERPORT_H

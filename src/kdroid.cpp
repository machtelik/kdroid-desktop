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

#include "kdroid.h"

#include <KCmdLineArgs>

KDroid::KDroid():
m_gui(new KDroidXmlGui())
{

}

KDroid::~KDroid()
{
  if(m_gui) {
    delete m_gui;
  }
}

int KDroid::newInstance()
{
    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    m_gui->show();
    args->clear();
    return KUniqueApplication::newInstance();
}



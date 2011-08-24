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
#include <KDE/KApplication>
#include <KDE/KAboutData>
#include <KDE/KCmdLineArgs>
#include <KDE/KLocale>

static const char description[] =
    I18N_NOOP("Android SMS Sync");

static const char version[] = "0.5";

int main(int argc, char **argv)
{
    KAboutData about("kdroid", 0, ki18n("KDroid"), version, ki18n(description),
                     KAboutData::License_GPL, ki18n("(C) 2011 Mike Achtelik"), KLocalizedString(), 0, "mike.achtelik+kdroidbugs@gmail.com");
    about.addAuthor( ki18n("Mike Achtelik"), KLocalizedString(), "mike.achtelik+kdroid@gmail.com" );
    KCmdLineArgs::init(argc, argv, &about);

    KCmdLineOptions options;
    KCmdLineArgs::addCmdLineOptions(options);
    KApplication app;

    KDroid *widget = new KDroid;

    if (app.isSessionRestored())
    {
        RESTORE(KDroid);
    }
    else
    {
        //KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
        widget->show();

    }

    return app.exec();
}

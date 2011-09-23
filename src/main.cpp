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

#include <KDE/KUniqueApplication>
#include <KDE/KAboutData>
#include <KDE/KCmdLineArgs>
#include <KDE/KLocale>

#include "kdroid.h"

static const char version[] = "0.8";

int main(int argc, char **argv)
{
    KAboutData about( "kdroid", 0, ki18n("KDroid"), version, ki18n("Android Text Message manager"),
                     KAboutData::License_GPL, ki18n("(C) 2011 Mike Achtelik"), KLocalizedString(), 0, "mike.achtelik+kdroidbugs@gmail.com" );
    about.addAuthor( ki18n("Mike Achtelik"), KLocalizedString(), "mike.achtelik+kdroid@gmail.com" );
    KCmdLineArgs::init(argc, argv, &about);

    KCmdLineOptions options;
    options.add("send",ki18n("Sends text message if <address> and <body> are provided"));
    options.add("address <address>",ki18n("Address"));
    options.add("body <body>",ki18n("Message body"));
    options.add("quiet",ki18n("Don't show the Main Window"));
    options.add("test <test>",ki18n("Test the (network) connection to the phone"));
    KCmdLineArgs::addCmdLineOptions(options);
    KCmdLineArgs::init(argc, argv, &about);
    KCmdLineArgs::addCmdLineOptions( options );
    KDroid::addCmdLineOptions();

    if (!KDroid::start()) {
        fprintf(stdout, "KDroid is already running!\n");
        fprintf(stdout, "Arguments have been passed to first instance\n");
        return 0;
    }
    KDroid app;
    return app.exec();
}

//==========================================================================
//
//==========================================================================
//  Copyright (c) DogRatIan.  All rights reserved.
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//==========================================================================
// Naming conventions
// ~~~~~~~~~~~~~~~~~~
//                Class : Leading C
//               Struct : Leading T
//             Constant : Leading K
//      Global Variable : Leading g
//    Function argument : Leading a
//       Local Variable : All lower case
//==========================================================================
#include <QCoreApplication>
#include <QDir>

#include "debug.h"

#include "qml_statistic.h"

//==========================================================================
// Defines
//==========================================================================
#define DEFAULT_DB_FILENAME "statistic.sqlite3"

//==========================================================================
// Constants
//==========================================================================


//==========================================================================
//==========================================================================
// Public members
// vvvvvvvvvvvvvv
//==========================================================================
//==========================================================================

//==========================================================================
// Constructor
//==========================================================================
CStatistic::CStatistic (QObject *aParent) :
    QObject(aParent) {
}

//==========================================================================
// Destroyer
//==========================================================================
CStatistic::~CStatistic (void) {
}


//==========================================================================
// Initialization
//==========================================================================
bool CStatistic::init (void) {
    try {
        QString sql;
        auto path = getPath ();

        sqlite3pp::database db (path.toUtf8().data());

        sql = "CREATE TABLE IF NOT EXISTS temperature_1min (timestamp INTEGER PRIMARY KEY UNIQUE NOT NULL,"
                "avg REAL, max REAL,min REAL);";
        DEBUG_PRINTF ("SQL:%s", sql.toUtf8().data());
        db.execute (sql.toUtf8().data());

        sql = "CREATE TABLE IF NOT EXISTS humidity_1min (timestamp INTEGER PRIMARY KEY UNIQUE NOT NULL,"
                "avg REAL, max REAL,min REAL);";
        DEBUG_PRINTF ("SQL:%s", sql.toUtf8().data());
        db.execute (sql.toUtf8().data());

        sql = "CREATE TABLE IF NOT EXISTS pressure_1min (timestamp INTEGER PRIMARY KEY UNIQUE NOT NULL,"
                "avg REAL, max REAL,min REAL);";
        DEBUG_PRINTF ("SQL:%s", sql.toUtf8().data());
        db.execute (sql.toUtf8().data());
    }
    catch (std::exception& aError) {
        emit errorMessage (QString ("CStatistic init error. %1").arg (aError.what()));
    }

    return true;
}

//==========================================================================
//==========================================================================
// Public members
// ^^^^^^^^^^^^^^^
//==========================================================================
//==========================================================================
// Private members
// vvvvvvvvvvvvvv
//==========================================================================
//==========================================================================

//==========================================================================
// Generate config file PATH
//==========================================================================
QString CStatistic::getPath (void) {
    QString path = QCoreApplication::applicationDirPath();
    path.append (QDir::separator());
    if (currentFilename.length() <= 0)
        path.append (DEFAULT_DB_FILENAME);
    else
        path.append (currentFilename);

    return path;
}


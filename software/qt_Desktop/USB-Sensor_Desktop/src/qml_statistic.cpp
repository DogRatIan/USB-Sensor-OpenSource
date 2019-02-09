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
#include <cmath>

#include "debug.h"

#include "qml_statistic.h"

//==========================================================================
// Defines
//==========================================================================
#define DEFAULT_DB_FILENAME "statistic.sqlite3"

// unit in second
#define TIME_AVERAGE_PERIOD     60

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
    databaseReady = false;
    clearFeedBuffer ();
    updateFileSize ();
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

        // Connecto to database file
        auto conn_ret = database.connect (path.toUtf8().data(), SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
        if (conn_ret != SQLITE_OK) {
            emit errorMessage ("Can't connect database.");
            return false;
        }
        database.set_busy_timeout (2000);


        sql = "CREATE TABLE IF NOT EXISTS temperature_1min (timestamp INTEGER PRIMARY KEY UNIQUE NOT NULL,"
                "avg REAL, max REAL,min REAL);";
        DEBUG_PRINTF ("SQL:%s", sql.toUtf8().data());
        database.execute (sql.toUtf8().data());

        sql = "CREATE TABLE IF NOT EXISTS humidity_1min (timestamp INTEGER PRIMARY KEY UNIQUE NOT NULL,"
                "avg REAL, max REAL,min REAL);";
        DEBUG_PRINTF ("SQL:%s", sql.toUtf8().data());
        database.execute (sql.toUtf8().data());

        sql = "CREATE TABLE IF NOT EXISTS pressure_1min (timestamp INTEGER PRIMARY KEY UNIQUE NOT NULL,"
                "avg REAL, max REAL,min REAL);";
        DEBUG_PRINTF ("SQL:%s", sql.toUtf8().data());
        database.execute (sql.toUtf8().data());

        //
        databaseReady = true;
        clearFeedBuffer ();
        updateFileSize ();
    }
    catch (std::exception& aError) {
        emit errorMessage (QString ("CStatistic init error. %1").arg (aError.what()));
    }

    return true;
}

//==========================================================================
// Clear feed buffer
//==========================================================================
void CStatistic::clearFeedBuffer (void) {
    timestampStart = 0;
    timestampEnd = 0;
    clearAllAveraging ();
    lastAverageTimestamp = 0;
    lastAvgTemperature = std::nan("");
    lastAvgHumidity = std::nan("");
    lastAvgPressure = std::nan("");
}

//==========================================================================
// Feed data for statistic
//==========================================================================
void CStatistic::feedData (time_t aTimestamp, double aTemperature, double aHumidity, double aPressure) {
    if (!databaseReady)
        return;

    DEBUG_PRINTF ("feedData %lu, %.2f, %.2f, %.2f", aTimestamp, aTemperature, aHumidity, aPressure);
    if ((aTimestamp < timestampStart) || (aTimestamp > timestampEnd)) {

        // Save averaging result
        if (timestampStart != timestampEnd) {
            lastAverageTimestamp = timestampEnd;
            lastAvgTemperature = saveAveraging (&avgTemperature, lastAverageTimestamp, "temperature_1min");
            lastAvgHumidity = saveAveraging (&avgHumidity, lastAverageTimestamp, "humidity_1min");
            lastAvgPressure = saveAveraging (&avgPressure, lastAverageTimestamp, "pressure_1min");
        }

        // Start new averaging period
        timestampStart = (aTimestamp / TIME_AVERAGE_PERIOD) * TIME_AVERAGE_PERIOD + 1;
        timestampEnd = timestampStart + TIME_AVERAGE_PERIOD - 1;
        DEBUG_PRINTF ("Average Period %lu to %lu", timestampStart, timestampEnd);

        clearAllAveraging ();
    }

    // Do averaging
    updateAveraging (&avgTemperature, aTemperature);
    updateAveraging (&avgHumidity, aHumidity);
    updateAveraging (&avgPressure, aPressure);
    updateFileSize ();
    emit feedDataFinished ();
}

//==========================================================================
// Read/Write properties
//==========================================================================
QString CStatistic::readFilename (void) {
    if (currentFilename.length() <= 0)
        return DEFAULT_DB_FILENAME;
    else
        return currentFilename;
}

double CStatistic::readFileSize (void) {
    return currentFileSize;
}


bool CStatistic::readIsReady (void) {
    return databaseReady;
}

long CStatistic::readAvaragePeriodLenght (void) {
    return TIME_AVERAGE_PERIOD;
}

long CStatistic::readCurrentPeriodStart (void) {
    return timestampStart;
}

long CStatistic::readCurrentPeriodEnd (void) {
    return timestampEnd;
}

int CStatistic::readAvgTemperatureCount (void) {
    return avgTemperature.dataCount;
}

int CStatistic::readAvgHumidityCount (void) {
    return avgHumidity.dataCount;
}

int CStatistic::readAvgPressureCount (void) {
    return avgPressure.dataCount;
}

double CStatistic::readLastTemperature (void) {
    return lastAvgTemperature;
}

double CStatistic::readLastHumidity (void) {
    return lastAvgHumidity;
}

double CStatistic::readLastPressure (void) {
    return lastAvgPressure;
}

long CStatistic::readLastTimestamp (void) {
    return lastAverageTimestamp;
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
    path.append (readFilename());
    return path;
}

//==========================================================================
// Update file size
//==========================================================================
void CStatistic::updateFileSize (void) {
    QFileInfo info (getPath ());
    if (info.exists()) {
        currentFileSize = static_cast<double>(info.size()) / 1024 / 1024;
    }
    else {
        currentFileSize = std::nan("");
    }
}


//==========================================================================
// Clear all averaging
//==========================================================================
void CStatistic::clearAllAveraging (void) {
    memset (&avgTemperature, 0, sizeof (struct TAveraging));
    memset (&avgHumidity, 0, sizeof (struct TAveraging));
    memset (&avgPressure, 0, sizeof (struct TAveraging));

    avgTemperature.dataSum = 0;
    avgHumidity.dataSum = 0;
    avgPressure.dataSum = 0;
    avgTemperature.min = std::nan ("");
    avgTemperature.max = std::nan ("");
    avgHumidity.min = std::nan ("");
    avgHumidity.max = std::nan ("");
    avgPressure.min = std::nan ("");
    avgPressure.max = std::nan ("");
}

//==========================================================================
// Update averaging data
//==========================================================================
void CStatistic::updateAveraging (struct TAveraging *aAveraging, double aNewValue) {
    if (!std::isnan (aNewValue)) {
        aAveraging->dataSum += aNewValue;
        aAveraging->dataCount ++;

        if (std::isnan (aAveraging->min))
            aAveraging->min = aNewValue;
        else if (aNewValue < aAveraging->min)
            aAveraging->min = aNewValue;
        if (std::isnan (aAveraging->max))
            aAveraging->max = aNewValue;
        else if (aNewValue > aAveraging->max)
            aAveraging->max = aNewValue;
    }
}

//==========================================================================
// Save averaging result
//==========================================================================
double CStatistic::saveAveraging (struct TAveraging *aAveraging, time_t aTimestamp, QString aTableName) {
    QString str_avg = "NULL";
    QString str_min = "NULL";
    QString str_max = "NULL";
    QString sql;
    double avg_value = std::nan ("");

    if (aAveraging->dataSum > 0) {
        avg_value = aAveraging->dataSum / aAveraging->dataCount;
        str_avg.sprintf ("%.2f", avg_value);
    }
    if (!std::isnan(aAveraging->min)) {
        str_min.sprintf ("%.2f", aAveraging->min);
    }
    if (!std::isnan(aAveraging->max)) {
        str_max.sprintf ("%.2f", aAveraging->max);
    }

    try {
        sql = QString ("INSERT INTO %1 (timestamp,avg,min,max) VALUES (%2,%3,%4,%5);").arg (aTableName)
                .arg (aTimestamp).arg(str_avg).arg(str_min).arg(str_max);
        DEBUG_PRINTF ("SQL:%s", sql.toUtf8().data());
        database.execute (sql.toUtf8().data());
    }
    catch (std::exception& aError) {
        emit errorMessage (QString ("CStatistic save error. %1").arg (aError.what()));
    }

    return avg_value;
}

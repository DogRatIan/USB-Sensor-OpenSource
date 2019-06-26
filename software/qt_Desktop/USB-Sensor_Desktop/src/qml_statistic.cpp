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
#include <QDateTime>
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
//bool CStatistic::init (QList<QString>aName, QList<QString>aShortName) {
bool CStatistic::init (void) {
    DEBUG_PRINTF ("init() nameList len=%d", nameList.length());
    try {
        QString sql;
        auto path = getPath ();

        // Clear averaging
        averageList.clear();

//        // Save name List
//        nameList = aName;
//        shortNameList = aShortName;


        // Connecto to database file
        auto conn_ret = database.connect (path.toUtf8().data(), SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
        if (conn_ret != SQLITE_OK) {
            emit errorMessage ("Can't connect database.");
            return false;
        }
        database.set_busy_timeout (2000);

        for (int i = 0; i < nameList.length(); i ++) {
            struct TAveraging averaging;
            averageList.push_front(averaging);

            // Create DB tables
            QString reading_name = nameList.at(i);
            if (reading_name.length() == 0)
                continue;

            sql = QString ("CREATE TABLE IF NOT EXISTS %1_1min (timestamp INTEGER PRIMARY KEY UNIQUE NOT NULL,"
                    "avg REAL, max REAL,min REAL);").arg (reading_name);
            DEBUG_PRINTF ("SQL:%s", sql.toUtf8().data());
        }

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
    clearAllAveraging ();

    timestampStart = 0;
    timestampEnd = 0;
    emit averagePeriodChanged ("");

    lastSavedValues.clear();
    emit lastSavedChanged ("", lastSavedValues);

    lastDataCount.clear();
    emit dataCountChanged (lastDataCount);

}

//==========================================================================
// Feed a data
//==========================================================================
void CStatistic::feedData (long aTimestamp, QString aName, double aValue ) {
    int index = nameList.indexOf (aName);
    if (index < 0) {
        DEBUG_PRINTF ("Reading %s not found.", qUtf8Printable(aName));
        return;
    }

    QString short_name = aName;
    if (index < shortNameList.length()) {
        short_name = shortNameList.at(index);
    }

    DEBUG_PRINTF ("feedData %s, %lu, %.2f", qUtf8Printable(aName), aTimestamp, aValue);

    auto avg_info = averageList.at (index);

    if ((aTimestamp < timestampStart) || (aTimestamp > timestampEnd)) {

        // Save averaging result
        if (timestampStart != timestampEnd) {
            QString table_name = aName + "_1min";
            auto last_save_value = saveAveraging (&avg_info, timestampEnd, table_name);

            QString str_value;
            str_value.sprintf ("%s=%.2f", qUtf8Printable(short_name), last_save_value);
            if (lastSavedValues.length() > 0) {
                lastSavedValues.append (", ");
            }
            lastSavedValues.append (str_value);
        }

        clearAveraging (&averageList[index]);
    }

    updateAveraging (&averageList[index], aValue);

    QString str_count;
    str_count.sprintf ("%s=%d", qUtf8Printable(short_name), averageList[index].dataCount);
    if (lastDataCount.length() > 0) {
        lastDataCount.append (", ");
    }
    lastDataCount.append (str_count);
}

//==========================================================================
// End of Feed data
//==========================================================================
void CStatistic::feedData (long aTimestamp) {
    updateFileSize ();

    if ((aTimestamp < timestampStart) || (aTimestamp > timestampEnd)) {
        // Start new averaging period
        time_t start = (aTimestamp / TIME_AVERAGE_PERIOD) * TIME_AVERAGE_PERIOD + 1;
        timestampStart = start;
        timestampEnd = start + TIME_AVERAGE_PERIOD - 1;
        DEBUG_PRINTF ("  Average Period %lu to %lu", timestampStart, timestampEnd);

        QDateTime date_start;
        QDateTime date_end;
        date_start.setSecsSinceEpoch (timestampStart);
        date_end.setSecsSinceEpoch (timestampEnd);
        QString str_period = QString ("%1 to %2").arg (date_start.toString ("HH:mm:ss")).arg (date_end.toString ("HH:mm:ss"));
        DEBUG_PRINTF ("  Average Period %s", qUtf8Printable(str_period));
        emit averagePeriodChanged (str_period);

        DEBUG_PRINTF ("  Last saved: %s", qUtf8Printable(lastSavedValues));
        if (lastSavedValues.length() > 0)
            emit lastSavedChanged (date_end.toString ("HH:mm:ss"), lastSavedValues);
        else
            emit lastSavedChanged ("", lastSavedValues);
        lastSavedValues.clear();
    }

    DEBUG_PRINTF ("  Last data count: %s", qUtf8Printable(lastDataCount));
    emit dataCountChanged (lastDataCount);
    lastDataCount.clear();

}


//==========================================================================
// Feed data for statistic for USB-PA and USB-TnH
//==========================================================================
//void CStatistic::feedData_PA (time_t aTimestamp, double aTemperature, double aHumidity, double aPressure) {
//    if (!databaseReady)
//        return;

//    DEBUG_PRINTF ("feedData %lu, %.2f, %.2f, %.2f", aTimestamp, aTemperature, aHumidity, aPressure);
//    if ((aTimestamp < timestampStart) || (aTimestamp > timestampEnd)) {

//        // Save averaging result
//        if (timestampStart != timestampEnd) {
//            lastAverageTimestamp = timestampEnd;
//            lastAvgTemperature = saveAveraging (&avgTemperature, lastAverageTimestamp, "temperature_1min");
//            lastAvgHumidity = saveAveraging (&avgHumidity, lastAverageTimestamp, "humidity_1min");
//            lastAvgPressure = saveAveraging (&avgPressure, lastAverageTimestamp, "pressure_1min");
//        }

//        // Start new averaging period
//        timestampStart = (aTimestamp / TIME_AVERAGE_PERIOD) * TIME_AVERAGE_PERIOD + 1;
//        timestampEnd = timestampStart + TIME_AVERAGE_PERIOD - 1;
//        DEBUG_PRINTF ("Average Period %lu to %lu", timestampStart, timestampEnd);

//        clearAllAveraging ();
//    }

//    // Do averaging
//    updateAveraging (&avgTemperature, aTemperature);
//    updateAveraging (&avgHumidity, aHumidity);
//    updateAveraging (&avgPressure, aPressure);
//    updateFileSize ();
//    emit feedDataFinished ();
//}

//==========================================================================
// Remove old data
//==========================================================================
bool CStatistic::removeOldData (int aHoursAgo) {
    QString sql;
    bool ret = false;

    if (!databaseReady)
        return ret;

    try {
        if (aHoursAgo > 0) {
            // Calcualte target timestamp
            auto now = QDateTime::currentMSecsSinceEpoch() / 1000;
            now = now - (now % 3600);   // Round down to hour
            time_t timestamp = static_cast<time_t>(now) - (aHoursAgo * 3600);
            DEBUG_PRINTF ("removeOldData, target timesatmp=%ld", timestamp);

            sql = QString ("DELETE FROM temperature_1min WHERE timestamp <= %1;").arg(timestamp);
            DEBUG_PRINTF ("SQL:%s", sql.toUtf8().data());
            database.execute (sql.toUtf8().data());
            sql = QString ("DELETE FROM humidity_1min WHERE timestamp <= %1;").arg(timestamp);
            DEBUG_PRINTF ("SQL:%s", sql.toUtf8().data());
            database.execute (sql.toUtf8().data());
            sql = QString ("DELETE FROM pressure_1min WHERE timestamp <= %1;").arg(timestamp);
            DEBUG_PRINTF ("SQL:%s", sql.toUtf8().data());
            database.execute (sql.toUtf8().data());
        }
        else {
            sql = QString ("DELETE FROM temperature_1min;");
            DEBUG_PRINTF ("SQL:%s", sql.toUtf8().data());
            database.execute (sql.toUtf8().data());
            sql = QString ("DELETE FROM humidity_1min;");
            DEBUG_PRINTF ("SQL:%s", sql.toUtf8().data());
            database.execute (sql.toUtf8().data());
            sql = QString ("DELETE FROM pressure_1min;");
            DEBUG_PRINTF ("SQL:%s", sql.toUtf8().data());
            database.execute (sql.toUtf8().data());
        }

        // ALL done
        ret = true;
    }
    catch (std::exception& aError) {
        emit errorMessage (QString ("CStatistic remove error. %1").arg (aError.what()));
    }

    return ret;
}

//==========================================================================
// Export data
//=========================================================================
bool CStatistic::exportData (QString aTargetPath) {
    QFileInfo info (aTargetPath);
    if (!info.exists()) {
        emit errorMessage (QString ("Export fail, '%1' not exist").arg(aTargetPath));
        return false;
    }
    DEBUG_PRINTF ("Export to %s", aTargetPath.toUtf8().data());

    QString output_path;
    output_path = aTargetPath + QDir::separator() + "temperature_1min.csv";
    if (exportCsv (output_path, "temperature_1min") < 0)
        return false;
    output_path = aTargetPath + QDir::separator() + "humidity_1min.csv";
    if (exportCsv (output_path, "humidity_1min") < 0)
        return false;
    output_path = aTargetPath + QDir::separator() + "pressure_1min.csv";
    if (exportCsv (output_path, "pressure_1min") < 0)
        return false;

    return true;
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

//long CStatistic::readCurrentPeriodStart (void) {
//    return timestampStart;
//}

//long CStatistic::readCurrentPeriodEnd (void) {
//    return timestampEnd;
//}

//int CStatistic::readAvgTemperatureCount (void) {
//    return avgTemperature.dataCount;
//}

//int CStatistic::readAvgHumidityCount (void) {
//    return avgHumidity.dataCount;
//}

//int CStatistic::readAvgPressureCount (void) {
//    return avgPressure.dataCount;
//}

//double CStatistic::readLastTemperature (void) {
//    return lastAvgTemperature;
//}

//double CStatistic::readLastHumidity (void) {
//    return lastAvgHumidity;
//}

//double CStatistic::readLastPressure (void) {
//    return lastAvgPressure;
//}

//long CStatistic::readLastTimestamp (void) {
//    return lastAverageTimestamp;
//}

QList<QString> CStatistic::readValueNames (void) {
    return nameList;
}

void CStatistic::writeValueNames (QList<QString>aNames) {
    nameList = aNames;
}

void CStatistic::writeValueShortNames (QList<QString>aShortNames) {
    shortNameList = aShortNames;
}

QList<QString> CStatistic:: readValueShortNames (void) {
    return shortNameList;
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
// Clear averaging data
//==========================================================================
void CStatistic::clearAveraging (struct TAveraging *aAvg) {
    aAvg->dataCount = 0;
    aAvg->dataSum = 0;
    aAvg->min = std::nan ("");
    aAvg->max = std::nan ("");
}

//==========================================================================
// Clear all averaging
//==========================================================================
void CStatistic::clearAllAveraging (void) {
//    memset (&avgTemperature, 0, sizeof (struct TAveraging));
//    memset (&avgHumidity, 0, sizeof (struct TAveraging));
//    memset (&avgPressure, 0, sizeof (struct TAveraging));

//    avgTemperature.dataSum = 0;
//    avgHumidity.dataSum = 0;
//    avgPressure.dataSum = 0;
//    avgTemperature.min = std::nan ("");
//    avgTemperature.max = std::nan ("");
//    avgHumidity.min = std::nan ("");
//    avgHumidity.max = std::nan ("");
//    avgPressure.min = std::nan ("");
//    avgPressure.max = std::nan ("");

    for (int i = 0; i < averageList.length(); i ++) {
        clearAveraging (&averageList[i]);
    }

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

//=========================================================================
// Export to CSV file
//=========================================================================
int CStatistic::exportCsv (QString aOutputPath, QString aTableName) {
    QString sql;
    QString str_output;
    QFile output_file (aOutputPath);
    QDateTime row_date;
    int ret = -1;

    do {
        if (!output_file.open(QIODevice::WriteOnly)) {
            emit errorMessage ("Unable open file for data export.");
            break;
        }

        str_output = "timestamp, avg, min, max\n";
        output_file.write(str_output.toUtf8());

        try {
            sql = QString ("SELECT timestamp,avg,min,max FROM %1 ORDER BY timestamp;").arg(aTableName);
            DEBUG_PRINTF ("SQL:%s", sql.toUtf8().data());
            sqlite3pp::query qry (database, sql.toUtf8().data());

            for (auto row = qry.begin(); row != qry.end(); ++row) {
                auto timestamp = (*row).get<long long>(0);
                QString str_avg = "NaN";
                QString str_min = "NaN";
                QString str_max = "NaN";

                if ((*row).get<char const*>(1) != nullptr)
                    str_avg.sprintf ("%.2f", (*row).get<double>(1));
                if ((*row).get<char const*>(2) != nullptr)
                    str_min.sprintf ("%.2f", (*row).get<double>(2));
                if ((*row).get<char const*>(3) != nullptr)
                    str_max.sprintf ("%.2f", (*row).get<double>(3));

                row_date.setMSecsSinceEpoch (timestamp * 1000);
                auto str_date = row_date.toString(Qt::SystemLocaleShortDate);

                str_output = QString ("%1, %2, %3, %4\n").arg(str_date).arg(str_avg).arg(str_min).arg(str_max);
                output_file.write(str_output.toUtf8());
            }

        }
        catch (std::exception& aError) {
            emit errorMessage (QString ("CStatistic export data. %1").arg (aError.what()));
        }

        // ALL done
        ret = 0;
    } while (0);
    output_file.close ();

    return ret;
}

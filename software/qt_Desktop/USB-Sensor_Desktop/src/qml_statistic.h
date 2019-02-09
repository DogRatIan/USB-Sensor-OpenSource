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
#ifndef INC_QML_STATISTIC_H
#define INC_QML_STATISTIC_H

//==========================================================================
//==========================================================================
#include <QObject>

#include "sqlite3pp.h"

//==========================================================================
// Defines
//==========================================================================
struct TAveraging {
    int dataCount;
    double dataSum;
    double min;
    double max;
};

//==========================================================================
//==========================================================================
class CStatistic : public QObject {
    Q_OBJECT

public:
    explicit CStatistic (QObject *aParent = nullptr);
     ~CStatistic (void);

    // Methods for QML
    Q_INVOKABLE bool init (void);
    Q_INVOKABLE void clearFeedBuffer (void);
    Q_INVOKABLE void feedData (long aTimestamp, double aTemperature, double aHumidity, double aPressure);

    // Properties for QML
    Q_PROPERTY (QString filename READ readFilename)
    Q_PROPERTY (double fileSize READ readFileSize)
    Q_PROPERTY (bool isReady READ readIsReady)
    Q_PROPERTY (long avaragePeriodLenght READ readAvaragePeriodLenght)
    Q_PROPERTY (long currentPeriodStart READ readCurrentPeriodStart)
    Q_PROPERTY (long currentPeriodEnd READ readCurrentPeriodEnd)
    Q_PROPERTY (int avgTemperatureCount READ readAvgTemperatureCount)
    Q_PROPERTY (int avgHumidityCount READ readAvgHumidityCount)
    Q_PROPERTY (int avgPressureCount READ readAvgPressureCount)
    Q_PROPERTY (double lastTemperature READ readLastTemperature)
    Q_PROPERTY (double lastHumidity READ readLastHumidity)
    Q_PROPERTY (double lastPressure READ readLastPressure)
    Q_PROPERTY (long lastTimestamp READ readLastTimestamp)



    //
    QString readFilename (void);
    double readFileSize (void);
    bool readIsReady (void);
    long readAvaragePeriodLenght (void);
    long readCurrentPeriodStart (void);
    long readCurrentPeriodEnd (void);
    int readAvgTemperatureCount (void);
    int readAvgHumidityCount (void);
    int readAvgPressureCount (void);
    double readLastTemperature (void);
    double readLastHumidity (void);
    double readLastPressure (void);
    long readLastTimestamp (void);

    //

signals:
    void message (const QString aMessage);
    void errorMessage (const QString aMessage);

    void feedDataFinished (void);

private:
    QString currentFilename;
    double currentFileSize;
    sqlite3pp::database database;
    bool databaseReady;

    time_t timestampStart;
    time_t timestampEnd;

    struct TAveraging avgTemperature;
    struct TAveraging avgHumidity;
    struct TAveraging avgPressure;

    time_t lastAverageTimestamp;
    double lastAvgTemperature;
    double lastAvgHumidity;
    double lastAvgPressure;

    QString getPath (void);
    void updateFileSize (void);
    void clearAllAveraging (void);
    void updateAveraging (struct TAveraging *aAveraging, double aNewValue);
    double saveAveraging (struct TAveraging *aAveraging, time_t aTimestamp, QString aTableName);
};

//==========================================================================
//==========================================================================
#endif

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
    Q_INVOKABLE void feedData (long aTimestamp, QString aName, double aValue );
    Q_INVOKABLE void feedData (long aTimestamp);

    Q_INVOKABLE bool removeOldData (int aHoursAgo);
    Q_INVOKABLE bool exportData (QString aTargetPath);

    // Properties for QML
    Q_PROPERTY (QString filename READ readFilename)
    Q_PROPERTY (double fileSize READ readFileSize)
    Q_PROPERTY (bool isReady READ readIsReady)
    Q_PROPERTY (long avaragePeriodLenght READ readAvaragePeriodLenght)
    Q_PROPERTY (QList<QString> valueNames READ readValueNames WRITE writeValueNames)
    Q_PROPERTY (QList<QString> valueShortNames READ readValueShortNames WRITE writeValueShortNames)
    Q_PROPERTY (QList<QString> valueUnits READ readValueUnits WRITE writeValueUnits)


    //
    QString readFilename (void);
    double readFileSize (void);
    bool readIsReady (void);
    long readAvaragePeriodLenght (void);
    QList<QString> readValueNames (void);
    void writeValueNames (QList<QString>aNames);
    QList<QString> readValueShortNames (void);
    void writeValueShortNames (QList<QString>aShortNames);
    QList<QString> readValueUnits (void);
    void writeValueUnits (QList<QString>aUnits);


signals:
    void message (const QString aMessage);
    void errorMessage (const QString aMessage);

    void averagePeriodChanged (QString aPeriod);
    void lastSavedChanged (QString aTime, QString aValue);
    void dataCountChanged (QString aDataCount);

private:
    QString currentFilename;
    double currentFileSize;
    sqlite3pp::database database;
    bool databaseReady;

    time_t timestampStart;
    time_t timestampEnd;

    QList<QString> nameList;
    QList<QString> shortNameList;
    QList<QString> unitList;
    QList<struct TAveraging> averageList;

    QString lastSavedValues;
    QString lastDataCount;


    QString getPath (void);
    void updateFileSize (void);

    void clearAveraging (struct TAveraging *aAvg);
    void clearAllAveraging (void);
    void updateAveraging (struct TAveraging *aAveraging, double aNewValue);
    double saveAveraging (struct TAveraging *aAveraging, time_t aTimestamp, QString aTableName);
    int exportCsv (QString aOutputPath, QString aTableName, QString aTitle);
};

//==========================================================================
//==========================================================================
#endif

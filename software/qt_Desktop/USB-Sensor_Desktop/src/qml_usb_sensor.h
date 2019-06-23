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
#ifndef INC_QML_USB_SENSOR_H
#define INC_QML_USB_SENSOR_H

//==========================================================================
//==========================================================================
#include <QObject>
#include <QSerialPort>
#include <QJsonObject>

//==========================================================================
// Defines
//==========================================================================

//==========================================================================
//==========================================================================
class CUsbSensor : public QObject {
    Q_OBJECT

public:
    explicit CUsbSensor (QObject *aParent = nullptr);
     ~CUsbSensor (void);

    // Methods for QML
    Q_INVOKABLE bool open (QString aPortName);
    Q_INVOKABLE void close (void);
    Q_INVOKABLE bool isOpened (void);
    Q_INVOKABLE bool testPort (QString aPortName);
    Q_INVOKABLE bool update (void);

    // Properties for QML
    Q_PROPERTY (QString portName READ readPortName)
    Q_PROPERTY (QString deviceId READ readDeviceId)
    Q_PROPERTY (QString deviceVersion READ readDeviceVersion)
    Q_PROPERTY (QString deviceType READ readDeviceType)
//    Q_PROPERTY (double temperature READ readTemperature)
//    Q_PROPERTY (double humidity READ readHumidity)
//    Q_PROPERTY (double pressure READ readPressure)
//    Q_PROPERTY (bool hasTemperature READ readHasTemperature)
//    Q_PROPERTY (bool hasHumudity READ readHasHumidity)
//    Q_PROPERTY (bool hasPressure READ readHasPressure)
    Q_PROPERTY (QString result READ readResult)

    //
    QString readPortName (void);
    QString readDeviceId (void);
    QString readDeviceVersion (void);
    QString readDeviceType (void);
//    double readTemperature (void);
//    double readHumidity (void);
//    double readPressure (void);
//    bool readHasTemperature (void);
//    bool readHasHumidity (void);
//    bool readHasPressure (void);
    QString readResult (void);


    //

signals:
    void message (const QString aMessage);
    void errorMessage (const QString aMessage);

private:
    QString currentPortName;
    QSerialPort serialPort;
    QString currentDeviceId;
    QString currentDeviceVersion;
    QString currentDeviceType;
//    double currentTemperature;
//    double currentHumidity;
//    double currentPressure;
//    bool currentHasTemperature;
//    bool currentHasHumidity;
//    bool currentHasPressure;
    bool usingOldTnHGet;
    QJsonObject currentJsonResult;


    int openSerialPort (QString aPortName);
    int sendCommand (const char *aCmd);
    int waitResponse (char *aResp, int aRespSize, int aTimeout = -1);
    void clearReading (void);
    int getJsonReading (void);
    int getOldTnHReading (void);
};

//==========================================================================
//==========================================================================
#endif

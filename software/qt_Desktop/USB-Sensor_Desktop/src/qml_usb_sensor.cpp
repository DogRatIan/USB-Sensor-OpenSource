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
#include <QElapsedTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>

#include "debug.h"
#include "qml_usb_sensor.h"

//==========================================================================
// Defines
//==========================================================================
#define TIMEOUT_NO_RESPONSE     500

//==========================================================================
// Constants
//==========================================================================


//==========================================================================
//==========================================================================
static void FixOldFloatString (char *aStr, int aLen) {
    int i;
    int int_part = 0;
    int dec_part = 0;

    // Find negative sign
    for (i = 0; aStr[i]; i ++ ) {
        if (aStr[i] == '-')
            break;
    }
    if (aStr[i] == 0)
        return;

    // Find the .
    for (i = 0; aStr[i]; i ++ ) {
        if (aStr[i] == '.')
            break;
    }
    int_part = atoi (aStr);
    if (i == aLen) {
    }
    else {
        dec_part = atoi (&aStr[i + 1]);
    }

    if (int_part < 0)
        int_part *= -1;
    if (dec_part < 0)
        dec_part *= -1;
    dec_part ++;
    DEBUG_PRINTF ("int_part=%d\n", int_part);
    DEBUG_PRINTF ("dec_part=%d\n", dec_part);

    snprintf (aStr, static_cast<size_t>(aLen), "-%d.%02d", int_part, dec_part);
}

//==========================================================================
//==========================================================================
// Public members
// vvvvvvvvvvvvvv
//==========================================================================
//==========================================================================

//==========================================================================
// Constructor
//==========================================================================
CUsbSensor::CUsbSensor (QObject *aParent) :
    QObject(aParent) {
}

//==========================================================================
// Destroyer
//==========================================================================
CUsbSensor::~CUsbSensor (void) {
}

//==========================================================================
// Open port
//==========================================================================
bool CUsbSensor::open (QString aPortName) {
    DEBUG_PRINTF ("CUsbSensor::open %s", aPortName.toUtf8().data());

    clearReading();

    if (!testPort(aPortName)) {
        return false;
    }

    if (openSerialPort(aPortName) < 0) {
        return false;
    }
    else {
        currentPortName = aPortName;
        emit message (QString ("%1 opened.").arg(aPortName));
        return true;
    }    
}

//==========================================================================
// Close port
//==========================================================================
void CUsbSensor::close (void) {
    currentPortName.clear();
    serialPort.close ();
    clearReading();
}

//==========================================================================
// Port is opened?
//==========================================================================
bool CUsbSensor::isOpened (void) {
    return serialPort.isOpen();
}


//==========================================================================
// Read/Write properties
//==========================================================================
QString CUsbSensor::readPortName (void) {
    return currentPortName;
}

QString CUsbSensor::readDeviceId (void) {
    return currentDeviceId;
}

QString CUsbSensor::readDeviceVersion (void) {
    return currentDeviceVersion;
}

double CUsbSensor::readTemperature (void) {
    return currentTemperature;
}

double CUsbSensor::readHumidity (void) {
    return currentHumidity;
}

double CUsbSensor::readPressure (void) {
    return currentPressure;
}

bool CUsbSensor::readHasTemperature (void) {
    return currentHasTemperature;
}

bool CUsbSensor::readHasHumidity (void) {
    return currentHasHumidity;
}

bool CUsbSensor::readHasPressure (void) {
    return currentHasPressure;
}


//==========================================================================
// Test Port
//==========================================================================
bool CUsbSensor::testPort (QString aPortName) {
    QString str_resp;
    char resp[64];
    int resp_len;
    bool port_ok;

    DEBUG_PRINTF ("testPort");

    // Clear info
    currentDeviceId.clear();
    currentDeviceVersion.clear();
    currentHasTemperature = false;
    currentHasHumidity = false;
    currentHasPressure = false;

    // Uncoditional close
    close ();

    // Open Port
    if (openSerialPort (aPortName) < 0) {
        DEBUG_PRINTF ("Open port %s fail.", aPortName.toUtf8().data());
        emit errorMessage (QString ("%1 open fail: %2").arg(aPortName).arg(serialPort.errorString ()));
        return false;
    }
    emit message (QString ("%1 opened for test").arg(aPortName));

    //
    port_ok = false;
    do {
        // Get ID
        if (sendCommand("GI") < 0)
            break;
        resp_len = waitResponse(resp, sizeof (resp));
        if (resp_len < 0)
            break;
        str_resp = QString (resp);
        str_resp.replace("\n", "");
        str_resp.replace("\a", "");
        if (str_resp.startsWith ("USB-TnH")) {
            currentDeviceId = str_resp;
            if (str_resp.contains ("SHT30")) {
                usingGetJson = true;
            }
            else {
                usingGetJson = false;
            }
            currentHasTemperature = true;
            currentHasHumidity = true;
        }
        else if (str_resp.startsWith ("USB-PA")) {
            currentDeviceId = str_resp;
            usingGetJson = true;
            currentHasTemperature = true;
            currentHasHumidity = true;
            currentHasPressure = true;
        }
        else {
            emit errorMessage (QString ("Unknown device '%1'").arg(str_resp));
            break;
        }

        if (sendCommand("GV") < 0)
            break;
        resp_len = waitResponse(resp, sizeof (resp));
        if (resp_len < 0)
            break;
        currentDeviceVersion = QString (resp);
        currentDeviceVersion.replace("\n", "");
        currentDeviceVersion.replace("\a", "");

        if (currentDeviceId.startsWith("USB-PA")) {
            if (currentDeviceVersion.compare("V2,00") == 9) {
                // Using old get reading method that with float number string fixing
                usingGetJson = false;
            }
        }

        port_ok = true;
        emit message ("Port test success.");

    }while (0);

    serialPort.close ();

    return port_ok;
}

//==========================================================================
// Update reading
//==========================================================================
bool CUsbSensor::update (void) {
    if (!serialPort.isOpen()) {
        emit errorMessage ("Port not opened.");
        return false;
    }

    clearReading();

    if (usingGetJson) {
        if (getJsonReading() < 0)
            return false;
        else
            return true;
    }
    else {
        if (getReading() < 0)
            return false;
        else
            return true;
    }
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
// Open Port
//==========================================================================
int CUsbSensor::openSerialPort (QString aPortName)
{
    // Unconditional close
    serialPort.close ();

    // Open and setup port
    serialPort.setPortName (aPortName);
    if (!serialPort.open (QIODevice::ReadWrite))
    {
        auto msg = QString ("%1 ERROR: %2").arg(aPortName).arg(serialPort.errorString ());
        emit errorMessage (msg);
        return -1;
    }

    if (!serialPort.setBaudRate (QSerialPort::Baud115200))
    {
        auto msg = QString ("%1 ERROR: %2").arg(aPortName).arg(serialPort.errorString ());
        emit errorMessage (msg);
        return -1;
    }

    if (!serialPort.setDataBits (QSerialPort::Data8))
    {
        auto msg = QString ("%1 ERROR: %2").arg(aPortName).arg(serialPort.errorString ());
        emit errorMessage (msg);
        return -1;
    }

    if (!serialPort.setParity (QSerialPort::NoParity))
    {
        auto msg = QString ("%1 ERROR: %2").arg(aPortName).arg(serialPort.errorString ());
        emit errorMessage (msg);
        return -1;
    }

    if (!serialPort.setStopBits (QSerialPort::OneStop))
    {
        auto msg = QString ("%1 ERROR: %2").arg(aPortName).arg(serialPort.errorString ());
        emit errorMessage (msg);
        return -1;
    }

    if (!serialPort.setFlowControl(QSerialPort::NoFlowControl))
    {
        auto msg = QString ("%1 ERROR: %2").arg(aPortName).arg(serialPort.errorString ());
        emit errorMessage (msg);
        return -1;
    }

    serialPort.setDataTerminalReady (true);
    serialPort.flush ();

    return 0;
}

//==========================================================================
// Send command
//==========================================================================
int CUsbSensor::sendCommand (const char *aCmd) {
    char buf[32];

    memset (buf, 0, sizeof (buf));
    snprintf (buf, sizeof (buf) - 1, "%s\n", aCmd);
    DEBUG_PRINTF ("TX: %s", aCmd);
    if (serialPort.write (buf, qstrlen (buf)) < 0) {
        emit errorMessage (QString ("%1 ERROR: %2").arg(serialPort.portName()).arg(serialPort.errorString ()));
        return -1;
    }

    return 0;
}

//==========================================================================
// Wait Response
//==========================================================================
int CUsbSensor::waitResponse (char *aResp, int aRespSize, int aTimeout) {
    QElapsedTimer timer;
    char buf[16];
    QString str_resp;

    if (aTimeout < 0)
        aTimeout = TIMEOUT_NO_RESPONSE;

    timer.restart ();
    while (1)
    {
        memset (buf, 0, sizeof (buf));
        serialPort.waitForReadyRead (0);
        auto buf_len = serialPort.read (buf, sizeof (buf) - 1);
        if (buf_len < 0) {
            emit errorMessage (QString ("%1 ERROR: %2").arg(serialPort.portName()).arg(serialPort.errorString ()));
            break;
        }
        else if (buf_len == 0) {
            if (timer.elapsed() >= TIMEOUT_NO_RESPONSE) {
                emit errorMessage (QString ("%1 no response timeout.").arg(serialPort.portName()));
                return -1;
            }
            continue;
        }

        DEBUG_PRINTF ("Rxed %d", buf_len);
        DEBUG_HEX2STRING (buf, static_cast<int>(buf_len));
        str_resp.append (buf);

        if (str_resp.contains('\n')) {
            DEBUG_PRINTF ("Resp: %s", str_resp.toUtf8().data());
            DEBUG_HEX2STRING (str_resp.toUtf8().data(), str_resp.length());
            qstrncpy (aResp, str_resp.toUtf8().data(), static_cast<size_t>(aRespSize));
            return static_cast<int>(qstrlen (aResp));
        }
    }

    return 0;
}

//==========================================================================
// Clear reading
//==========================================================================
void CUsbSensor::clearReading (void) {
    currentTemperature = std::nan("");
    currentHumidity = std::nan("");
    currentPressure = std::nan("");
}

//==========================================================================
// Get reading via JSON
//==========================================================================
int CUsbSensor::getJsonReading (void) {
    char resp[64];

    if (sendCommand("GJSON") < 0) {
        return -1;
    }
    auto resp_len = waitResponse (resp, sizeof (resp));
    if (resp_len < 0)
        return -1;
    auto str_resp = QString (resp);

    QJsonDocument j_doc = QJsonDocument::fromJson(str_resp.toUtf8());
    if (j_doc.isNull()) {
        emit errorMessage ("Invalid JSON response.");
        return -1;
    }

    QJsonObject j_obj = j_doc.object();
    if (j_obj.contains ("T")) {
        currentTemperature = j_obj["T"].toDouble ();
    }
    if (j_obj.contains ("H")) {
        currentHumidity = j_obj["H"].toDouble ();
    }
    if (j_obj.contains ("P")) {
        currentPressure = j_obj["P"].toDouble ();
    }
    DEBUG_PRINTF ("T=%.2f, H=%.2f, P=%.2f", currentTemperature, currentHumidity, currentPressure);

    return 0;
}

//==========================================================================
// Get reading via traditional commands
//==========================================================================
int CUsbSensor::getReading (void) {
    char resp[64];

    // Get Temperature
    if (currentHasTemperature) {
        if (sendCommand("GT") < 0) {
            return -1;
        }
        auto resp_len = waitResponse (resp, sizeof (resp));
        if (resp_len < 0)
            return -1;
        FixOldFloatString (resp, sizeof (resp));
        currentTemperature = atof (resp);
    }

    // Get Humidity
    if (currentHasHumidity) {
        if (sendCommand("GH") < 0) {
            return -1;
        }
        auto resp_len = waitResponse (resp, sizeof (resp));
        if (resp_len < 0)
            return -1;
        FixOldFloatString (resp, sizeof (resp));
        currentHumidity = atof (resp);
    }

    // Get Pressure
    if (currentHasPressure) {
        if (sendCommand("GP") < 0) {
            return -1;
        }
        auto resp_len = waitResponse (resp, sizeof (resp));
        if (resp_len < 0)
            return -1;
        FixOldFloatString (resp, sizeof (resp));
        currentPressure = atof (resp);
    }

    return 0;
}

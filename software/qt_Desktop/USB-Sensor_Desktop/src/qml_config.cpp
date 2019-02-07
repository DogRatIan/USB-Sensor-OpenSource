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
#include <QFileInfo>
#include <QDir>
#include <QCoreApplication>
#include <QJsonDocument>

#include "debug.h"

#include "qml_config.h"

//==========================================================================
// Defines
//==========================================================================
#define DEFAULT_CONFIG_FILENAME     "config.conf"

#define CONFIG_FILE_MAX_SIZE        (50 * 1024 * 1024)

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
CConfig::CConfig (QObject *aParent) :
    QObject(aParent) {
    currentFilename = DEFAULT_CONFIG_FILENAME;
}

//==========================================================================
// Destroyer
//==========================================================================
CConfig::~CConfig (void) {
}

//==========================================================================
// Check config file exist
//==========================================================================
bool CConfig::exists (void) {
    auto path = getPath ();
    QFileInfo config_file (path);
    return config_file.exists();
}

//==========================================================================
// Load Config
//==========================================================================
bool CConfig::load (void) {
    auto path = getPath ();
    DEBUG_PRINTF ("CConfig::load PATH=%s", path.toUtf8().data());

    QFileInfo config_file (path);
    if (!config_file.exists()) {
        DEBUG_PRINTF ("Config file not exist.");
        emit message ("Config file not exist.");
        return false;
    }

    QFile file (config_file.absoluteFilePath());
    if (!file.open(QIODevice::ReadOnly)) {
        DEBUG_PRINTF ("Config file open fail.");
        emit errorMessage ("Config file open fail.");
        return false;
    }

    QByteArray j_input = file.read (CONFIG_FILE_MAX_SIZE);
    file.close ();
    DEBUG_PRINTF ("Config file %d bytes read.", j_input.length());
    QJsonDocument j_doc = QJsonDocument::fromJson (j_input);
    if (j_doc.isNull ()) {
        DEBUG_PRINTF ("Invalid config file.");
        emit errorMessage ("Invalid config file.");
        return false;
    }

    jData = j_doc.object();
    emit message ("Config file loaded.");

    return true;
}

//==========================================================================
// Save Config
//==========================================================================
bool CConfig::save (void) {
    auto path = getPath ();
    DEBUG_PRINTF ("CConfig::save PATH=%s", path.toUtf8().data());

    // Prepare data for write
    QJsonDocument j_doc (jData);
    auto j_output = j_doc.toJson (QJsonDocument::Indented);


    // Open file
    QFileInfo config_file (path);
    QFile file (config_file.absoluteFilePath());
    if (!file.open (QIODevice::WriteOnly | QIODevice::Truncate)) {
        DEBUG_PRINTF ("Config file open fail.");
        emit errorMessage ("Config file open fail.");
    }

    // Write file
    file.write (j_output);
    file.close ();

    //
    emit message ("Config file saved.");
    return true;

}

//==========================================================================
// Read/Write properties
//==========================================================================
QString CConfig::readFilename (void) {
    return currentFilename;
}
void CConfig::writeFilename (QString aValue) {
    currentFilename = aValue;
}

//==========================================================================
// Set/Get a string data
//==========================================================================
bool CConfig::setStringData (QString aName, QString aValue) {
    if (jData.contains(aName)) {
        if (!jData[aName].isString()) {
            return false;
        }
    }
    jData[aName] = aValue;
    return true;
}

QString CConfig::getStringData (QString aName) {
    if ((jData.contains(aName)) && (jData[aName].isString())) {
        return jData[aName].toString();
    }
    else {
        return "";
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
// Generate config file PATH
//==========================================================================
QString CConfig::getPath (void) {
    QString path = QCoreApplication::applicationDirPath();
    path.append (QDir::separator());
    if (currentFilename.length() <= 0)
        path.append (DEFAULT_CONFIG_FILENAME);
    else
        path.append (currentFilename);

    return path;
}

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
#ifndef INC_QML_CONFIG_H
#define INC_QML_CONFIG_H

//==========================================================================
//==========================================================================
#include <QObject>
#include <QJsonObject>

//==========================================================================
// Defines
//==========================================================================

//==========================================================================
//==========================================================================
class CConfig : public QObject {
    Q_OBJECT

public:
    explicit CConfig (QObject *aParent = nullptr);
     ~CConfig (void);

    // Methods for QML
    Q_INVOKABLE bool exists (void);
    Q_INVOKABLE bool load (void);
    Q_INVOKABLE bool save (void);
    Q_INVOKABLE bool setStringData (QString aName, QString aValue);
    Q_INVOKABLE QString getStringData (QString aName);

    // Properties for QML
    Q_PROPERTY (QString filename READ readFilename WRITE writeFilename)


    //
    QString readFilename (void);
    void writeFilename (QString aValue);

    //

signals:
    void message (const QString aMessage);
    void errorMessage (const QString aMessage);

private:
    QString currentFilename;

    QString getPath (void);    

    QJsonObject jData;
};

//==========================================================================
//==========================================================================
#endif

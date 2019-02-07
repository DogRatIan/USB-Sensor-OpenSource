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
#ifndef INC_QML_PORT_LIST_H
#define INC_QML_PORT_LIST_H

//==========================================================================
//==========================================================================
#include <QObject>
#include <QString>
#include <QList>

//==========================================================================
// Defines
//==========================================================================
class CPortInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ readName)
    Q_PROPERTY(QString desc READ readDesc)

public:
    QString name;
    QString desc;

    QString readName (void) const {return name;}
    QString readDesc (void) const {return desc;}

};
Q_DECLARE_METATYPE(CPortInfo*)

//==========================================================================
//==========================================================================
class CPortList : public QObject
{
    Q_OBJECT

public:
    explicit CPortList (QObject *aParent = nullptr);
     ~CPortList (void);

    Q_INVOKABLE void refresh (void);
    Q_INVOKABLE CPortInfo *getItemAt (int aIdx);

signals:
    void changed (int aTotalCount);

private:
    QList <CPortInfo *> list;
};

//==========================================================================
//==========================================================================
#endif

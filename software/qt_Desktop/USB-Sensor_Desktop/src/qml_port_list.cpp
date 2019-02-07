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
#include <QGuiApplication>
#include <QtSerialPort/QSerialPortInfo>

#include "debug.h"

#include "qml_port_list.h"

//==========================================================================
//==========================================================================

//==========================================================================
// Constructor
//==========================================================================
CPortList::CPortList (QObject *aParent) :
    QObject(aParent)
{
    qRegisterMetaType<CPortInfo *>();

}

//==========================================================================
// Destroyer
//==========================================================================
CPortList::~CPortList (void)
{
//    exit ();
}

//==========================================================================
// Refresh port list
//==========================================================================
void CPortList::refresh (void)
{
    CPortInfo *curr_info;

    DEBUG_PRINTF ("CPortInfo refresh()");

    // Delete all items
    qDeleteAll(list.begin(), list.end());
    list.clear();

    // Add serial port name to Combobox
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {

        // Filter out ttyACM on linux
        if (QGuiApplication::platformName() == "xcb")
        {
            if (!info.portName().startsWith("ttyACM"))
                continue;
        }

        curr_info = new CPortInfo;
        curr_info->name.clear ();
        curr_info->desc.clear ();
        // Form port name
        curr_info->name = info.portName();
        if (info.description().length() > 0)
        {
            curr_info->desc = info.description();
        }

        if (info.manufacturer().length() > 0)
        {
            if (info.manufacturer().startsWith("("))
            {
                curr_info->desc.append (info.manufacturer());
            }
            else
            {
                curr_info->desc.append (" (");
                curr_info->desc.append (info.manufacturer());
                curr_info->desc.append (")");
            }
        }
        DEBUG_PRINTF ("Name=%s", curr_info->name.toUtf8().data());
        DEBUG_PRINTF ("Desc=%s", curr_info->desc.toUtf8().data());
        list.append (curr_info);
    }

    emit changed (list.count());
}

//==========================================================================
// Get info
//==========================================================================
CPortInfo *CPortList::getItemAt (int aIdx)
{
    if (aIdx >= list.count())
        return nullptr;

    return list[aIdx];
}


import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

import com.dogratian.qml.PortList 1.0
import "../LocalLib" as LocalLib


Page {
    id: rootPage
    title: qsTr("Config")

    property var targetPort;

    //==========================================================================
    // Functions
    //==========================================================================
    function reloadPortList () {
        itemPortList.refresh();
    }

    function testPort () {
        if (targetPort) {
            console.log ("Test " + targetPort.name);
            if (itemUsbSensor.testPort (targetPort.name)) {
                rootApp.showSystemMessage (qsTr ("INFO"), "Test success.\n"
                                           + itemUsbSensor.deviceId + " " + itemUsbSensor.deviceVersion + " found.");
            }
            else {
                rootApp.showSystemMessage (qsTr ("ERROR"), "Test fail.\n" + itemUsbSensor.lastErrorMessage);
            }
        }
        rootApp.showBusy (false);
    }

    //==========================================================================
    // ToolBar
    //==========================================================================
    header: ToolBar {
        id: itemTopToolBar
        ToolButton {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            icon.source: "qrc:/assets/back-arrow.svg"
            onClicked: navStack.pop ();
        }
        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            text: rootPage.title
        }
    }

    //==========================================================================
    // Main area
    //==========================================================================
    Item {
        anchors.fill: parent
        anchors.margins: 10

        Column {
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 5

            // Port List
            Item {
                anchors.left: parent.left
                anchors.right: parent.right
                height: 200

                Label {
                    id: textPortLabel
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.margins: 5
                    text: "PORT:"
                    font.bold: true
                    font.pointSize: 12
                    horizontalAlignment: Text.AlignRight
                }

                ToolButton {
                    id: buttonReload
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.margins: 5
                    icon.source: "qrc:/assets/reload.svg"
                    onClicked: reloadPortList();
                }


                Rectangle {
                    id: blockPortList
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.left: textPortLabel.right
                    anchors.right: buttonReload.left
                    anchors.margins: 5

                    ListView {
                        id: viewPortList
                        anchors.fill: parent
                        clip: true
                        model: listPort
                        delegate: Item {
                            width: viewPortList.width
                            height: 30
                            Row {
                                Text {
                                    text: name
                                    padding: 5
                                    font.pointSize: 12
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                Text {
                                    text: desc
                                    padding: 5
                                    font.pointSize: 12
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: viewPortList.currentIndex = index
                            }
                        }
                        highlight: Rectangle {
                            color: 'grey'
                            Text {
                                anchors.centerIn: parent
                            }
                        }
                    }

                }
            }

            // Button
            Button {
                anchors.right: parent.right
                width: parent.width * 0.5
                text: qsTr("Test")
                onClicked: {
                    rootApp.showBusy (true);
                    targetPort = itemPortList.getItemAt(viewPortList.currentIndex);
                    itemDelayFunc.forceSet (100, rootPage.testPort);
                }
            }
            Button {
                anchors.right: parent.right
                width: parent.width * 0.5
                text: qsTr("Set")
                onClicked: {
                    var selected_port = itemPortList.getItemAt(viewPortList.currentIndex);
                    console.log ("Set " + selected_port.name);
                    rootTop.selectedPort = selected_port;
                    navStack.pop ();

                    itemConfig.setStringData ("portName", selected_port.name)
                    itemConfig.setStringData ("portDesc", selected_port.desc)
                    itemConfig.save();
                }
            }

        }
    }

    //==========================================================================
    // Creation/Destruction
    //==========================================================================
    Component.onCompleted: {
        appendMessageToLog (objectName + " created.");
        reloadPortList();
    }

    //==========================================================================
    // C++ Object
    //==========================================================================
    PortList {
        id: itemPortList

        onChanged: {
            var i;

            console.log ("PortList onChanged " + aTotalCount);

            listPort.clear();
            for (i = 0; i < aTotalCount; i ++) {
                var info = itemPortList.getItemAt(i);
                listPort.append ({"name": info.name, "desc": info.desc});
                appendMessageToLog ("<span style='color:red;'>" + aTotalCount + qsTr ("</span> port(s) found."));
            }
        }
    }

    //==========================================================================
    // Delayed function
    //==========================================================================
    LocalLib.DelayFunc {
        id: itemDelayFunc
    }

    //==========================================================================
    // Models
    //==========================================================================
    ListModel {
        id: listPort
    }
}

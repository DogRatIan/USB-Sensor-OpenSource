import QtQuick 2.11
import QtQuick.Controls 2.4

//
Item {
    id: rootTop
    anchors.fill: parent

    //==========================================================================
    // Properties
    //==========================================================================

    //==========================================================================
    // Stack View
    //==========================================================================
    StackView {
        id: navStack;
        initialItem: pageHome
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: blockLog.top

        // Pages
        Component {
            id: pageHome
            PageHome {
                objectName: "pageHome"
            }
        }

        Component {
            id: pageConfig
            PageConfig {
                objectName: "pageConfig"
            }
        }

        Component.onCompleted: {
            // Setup log to current ListView
            viewMessageLog = messageLog;
            messageLog.positionViewAtEnd ();

            if (!itemConfig.exists()) {
                itemConfig.setStringData ("portName", "");
                itemConfig.setStringData ("portDesc", "");
                itemConfig.save();
                appendMessageToLog ("Config file created.");
            }

            if (itemConfig.load()) {
                var port_name = itemConfig.getStringData("portName");
                var port_desc = itemConfig.getStringData("portDesc");
                console.log ("Last port is " + port_name);
                selectedPort = {"name": port_name, "desc": port_desc};
            }

            itemStatistic.init ();
        }
        Component.onDestruction: {
        }
    }

    //==========================================================================
    // Message Log
    //==========================================================================
    Rectangle {
        id: blockLog
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        border.color: "grey"
        border.width: 1
        height: parent.height * 0.2

        ListView {
            id: messageLog
            model: listMessageLog
            anchors.fill: parent
            anchors.margins: 5
            clip: true

            delegate: Text {
                text: message
            }
        }
    }



}

import QtQuick 2.11
import QtQuick.Controls 2.4

//
Item {
    id: rootTop
    anchors.fill: parent

    //==========================================================================
    // Functions
    //==========================================================================
    function showMessageLog (aShow) {
        if (aShow) {
            blockLog.state = "show";
        }
        else {
            blockLog.state = "hide";
        }
    }

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

        Component {
            id: pageStatistic
            PageStatistic {
                objectName: "pageStatistic"
            }
        }

    }

    //==========================================================================
    // Message Log
    //==========================================================================
    Rectangle {
        id: blockLog
        anchors.left: parent.left
        anchors.right: parent.right
        border.color: "grey"
        border.width: 1
        y: rootTop.height - 100
        height: 100

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

        states: [
            State {
                name: "show"
                PropertyChanges {target: blockLog; y: rootTop.height - 100}
            },
            State {
                name: "hide"
                PropertyChanges {target: blockLog; y: rootTop.height}
            }
        ]

    }

    //==========================================================================
    // Creation/Destruction
    //==========================================================================
    Component.onCompleted: {
        // Setup log to current ListView
        viewMessageLog = messageLog;
        messageLog.positionViewAtEnd ();

        if (!itemConfig.exists()) {
            itemConfig.setStringData ("portName", "");
            itemConfig.setStringData ("portDesc", "");
            itemConfig.setStringData ("lastExportPath", "");
            itemConfig.save();
            appendMessageToLog ("Config file created.");
        }

        if (itemConfig.load()) {
            var port_name = itemConfig.getStringData("portName");
            var port_desc = itemConfig.getStringData("portDesc");
            console.log ("Last port is " + port_name);
            rootApp.selectedPort = {"name": port_name, "desc": port_desc};
        }

        var git_hash = itemConfig.getGitHash ();
        if (git_hash === "") {
            rootApp.gitHash = " ";
        }
        else {
            rootApp.gitHash = "(" + git_hash + ")";
        }

        itemStatistic.init ();
        rootTop.showMessageLog (false);
    }
    Component.onDestruction: {
    }
}

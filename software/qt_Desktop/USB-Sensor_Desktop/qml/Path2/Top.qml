import QtQuick 2.11
import QtQuick.Controls 2.4

//
Item {
    anchors.fill: parent

    SwipeView {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: blockLog.top

        Item {
            id: firstPage
            Column {
                Text {
                    text: "firstPage"
                }

                Button {
                    text: "RELOAD Normal"
                    onClicked: startNormal ();
                }
            }
        }
        Item {
            id: secondPage
            Column {
                Text {
                    text: "secondPage"
                }
            }
        }
        Item {
            id: thirdPage
            Column {
                Text {
                    text: "thirdPage"
                }
            }
        }
    }

    Component.onCompleted: {
        appendMessageToLog ("You are in Paht2 Top.");
        // Setup log to current ListView
        viewMessageLog = messageLog;
        messageLog.positionViewAtEnd ();
    }
    Component.onDestruction: {
    }

    // Message Log
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
            anchors.margins: 1
            clip: true

            delegate: Text {
                text: message
            }
        }
    }
}

import QtQuick 2.11
import QtQuick.Controls 2.4

ApplicationWindow {
    id: rootApp
    width: 550
    height: 550
    minimumWidth: 550
    minimumHeight: 550
    visible: true
    flags: Qt.FramelessWindowHint | Qt.Dialog
//    flags: Qt.Dialog
    title: qsTr("DogRatIan USB-Sensor (Desktop)")
    font.family: "Verdana"

    // Properties
    property var viewMessageLog

    // Function - Append to message log
    function appendMessageToLog (aMsg) {
        var msg = {message: aMsg};
        listMessageLog.append (msg);
        if (listMessageLog.count > 100)
            listMessageLog.remove (0);
        if ((viewMessageLog) && (typeof viewMessageLog.positionViewAtEnd == "function")) {
            viewMessageLog.positionViewAtEnd();
        }
    }

    // Function - Show/Hide Busy
    function showBusy (aShow) {
        indicatorBusy.visible = aShow;
    }

    // Function - Start Normal toppage
    function startNormal () {
        viewMessageLog = undefined;
        mainLoader.source = "";
        mainLoader.source = "Normal/Top.qml"
        console.log ("Load " + mainLoader.source);
    }

    // Function - Start Parth2
    function startPath2 () {
        viewMessageLog = undefined;
        mainLoader.source = "";
        mainLoader.source = "Path2/Top.qml"
        console.log ("Load " + mainLoader.source);
    }

    // Function - Show System Message
    function showSystemMessage (aTitle, aMessage) {
        showBusy (false);
        textSystemMessage.text = aMessage;
        dialogSystemMessage.title = aTitle;
        dialogSystemMessage.open();
    }


    // Models
    ListModel {
        id: listMessageLog
        ListElement {
            message: "Message log"
        }
    }

    // Top toolbar

    // Visual area
    Rectangle {
        anchors.fill: parent
        border.width: 1
        border.color: "#888888"
    }

    Item {
        anchors.fill: parent
        anchors.margins: 1

        Loader {
            id: mainLoader
            anchors.fill: parent
            focus: true
        }
    }

    // Busy indicator
    Rectangle {
        id: indicatorBusy
        visible: false
        anchors.fill: parent
        color: "#80000000"

        BusyIndicator  {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }

        MouseArea {
            anchors.fill: parent
        }
    }

    //
    Component.onCompleted: {
        startNormal ();
    }

    // Dialog
    Dialog {
        id: dialogSystemMessage
        title: "System Message"
        standardButtons: Dialog.Ok
        modal: true
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        implicitWidth: parent.width / 2
        implicitHeight: parent.height / 3

        header: ToolBar {
            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                text: dialogSystemMessage.title
            }
        }

        Label {
            id: textSystemMessage
            anchors.fill: parent
            anchors.margins: 5
            text: "-"
            wrapMode: Text.WordWrap
        }
    }
}

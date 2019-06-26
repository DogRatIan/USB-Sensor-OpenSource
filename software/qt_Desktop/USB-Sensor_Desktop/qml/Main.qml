import QtQuick 2.11
import QtQuick.Controls 2.4

import com.dogratian.qml.Config 1.0
import com.dogratian.qml.UsbSensor 1.0
import com.dogratian.qml.Statistic 1.0

ApplicationWindow {
    id: rootApp
    width: 600
    height: 600
    minimumWidth: 600
    minimumHeight: 600
    visible: true
    flags: Qt.FramelessWindowHint | Qt.Dialog
    title: qsTr("DogRatIan USB-Sensor (Desktop)")
    font.family: "Verdana"

    //==========================================================================
    // Properties
    //==========================================================================
    property var viewMessageLog
    property var selectedPort
    property string averagingPeriod: "Not Running"
    property string lastSavedValues: "-"
    property string lastSavedTime: "-"
    property string lastDataCount: "-"

    property string dbStatus: "-"
    property string dbFilename: "-"
    property string dbFileSize: "-"

    //==========================================================================
    // Functions
    //==========================================================================
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

    // Update DB info
    function updateDbInfo () {
        if (itemStatistic.isReady) {
            dbStatus = "Ready";
        }
        else {
            dbStatus = "Not connected.";
        }
        dbFilename = itemStatistic.filename;

        var file_size =  Number (itemStatistic.fileSize);
        if (file_size < 1) {
            file_size *= 1024;
            dbFileSize = file_size.toFixed(2) + " KiB";
        }
        else {
            dbFileSize = file_size.toFixed(2) + " MiB";
        }
    }

    //==========================================================================
    // Visual area
    //==========================================================================
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

    //==========================================================================
    // Busy indicator
    //==========================================================================
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

    //==========================================================================
    // Dialog - System Message
    //==========================================================================
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
            textFormat: Text.AutoText
            wrapMode: Text.WordWrap
        }
    }

    //==========================================================================
    // C++ Object
    //==========================================================================
    Config {
        id: itemConfig
        filename: "USB-Sensor_Desktop.conf"

        onMessage: {
            rootApp.appendMessageToLog (aMessage);
        }
        onErrorMessage: {
            rootApp.showSystemMessage (qsTr ("ERROR"), aMessage);
        }
    }

    UsbSensor {
        id: itemUsbSensor
        property string lastErrorMessage: " "

        onMessage: {
            rootApp.appendMessageToLog (aMessage);
        }

        onErrorMessage: {
            console.log ("UsbSensor ERROR: " + aMessage);
            lastErrorMessage = aMessage;
        }        
    }

    Statistic {
        id: itemStatistic

        valueNames: ["temperature", "humidity", "pressure", "tvoc", "co2eq"]
        valueShortNames: ["Temp", "Humi", "Pres", "TVOC", "CO2eq"]

        property var feedFinishedCallback

        onMessage: {
            rootApp.appendMessageToLog (aMessage);
        }
        onErrorMessage: {
            rootApp.showSystemMessage (qsTr ("ERROR"), aMessage);
        }

//        onFeedDataFinished: {
//            if (typeof (feedFinishedCallback) === "function") {
//                feedFinishedCallback ();
//            }
//        }

        onAveragePeriodChanged: {
            averagingPeriod = aPeriod;
            updateDbInfo ();
        }

        onLastSavedChanged: {
            lastSavedValues = aValue;
            lastSavedTime = aTime;
        }

        onDataCountChanged: {
            lastDataCount = aDataCount;
        }
    }

    //==========================================================================
    // Models
    //==========================================================================
    ListModel {
        id: listMessageLog
        ListElement {
            message: "Message log"
        }
    }

    //==========================================================================
    // Run on creation
    //==========================================================================
    Component.onCompleted: {
        startNormal ();
    }

}

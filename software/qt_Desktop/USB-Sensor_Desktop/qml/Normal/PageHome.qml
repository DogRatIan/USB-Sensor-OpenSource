import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.3

import "../LocalLib" as LocalLib

Page {
    id: rootPage
    title: qsTr("page1")

    //==========================================================================
    // Properties
    //==========================================================================
    property bool portOpened: false
    property string deviceName: " "

    //==========================================================================
    // Functions
    //==========================================================================
    function connectSensor () {
        if (rootApp.selectedPort === undefined) {
            rootApp.showSystemMessage ("ERROR", "Please select port first.");
            deviceName = " ";
            return;
        }
        if (rootApp.selectedPort !== undefined) {
            if (rootApp.selectedPort.name.length === 0) {
                rootApp.showSystemMessage ("ERROR", "Please select port first.");
                deviceName = " ";
                return;
            }
        }

        if (!itemUsbSensor.open (rootApp.selectedPort.name)) {
            rootApp.showSystemMessage ("ERROR", itemUsbSensor.lastErrorMessage);
            deviceName = " ";
            return;
        }
        portOpened = true;
        deviceName = itemUsbSensor.deviceId + " " + itemUsbSensor.deviceVersion;

        loaderReading.source = "";
        loaderCharting.source = "";
        switch (itemUsbSensor.deviceType) {
        case "USB-TnH":
            loaderReading.source = "Reading_PA.qml";
            loaderCharting.source = "Charting_TnH.qml";
            break;
        case "USB-PA":
            loaderReading.source = "Reading_PA.qml";
            loaderCharting.source = "Charting_PA.qml";
            break;
        case "USB-VOC":
            loaderReading.source = "Reading_VOC.qml";
            loaderCharting.source = "Charting_VOC.qml";
            break;
        }
    }


    //==========================================================================
    // ToolBar
    //==========================================================================
    header: TitleBar {
        titleText: rootApp.title

        leftLogoVisible: true
        leftLogoIconSource: "../../assets/app_icon.png"
        onLeftButtonClicked: itemDrawer.open();

        rightButtonVisible: true
        rightButtonEnabled: !rootPage.portOpened
        rightButtonIconSource: "../../assets/settings-gears.svg"
        onRightButtonClicked: {
            navStack.push (pageConfig);
            rootTop.showMessageLog (false);
        }
    }

    Drawer {
        id: itemDrawer
        width: rootPage.width * 0.20
        height: rootPage.height

        background: Rectangle {
            anchors.fill: parent
            border.width: 1
            border.color: "black"
        }

        Column {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: boxVersion.top

            Label {
                text: qsTr ("MENU")
                font.bold: true
                width: parent.width
                height: 30
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Rectangle {
                border.color: "black"
                border.width: 1
                height: 2
                width: parent.width
            }

            ItemDelegate {
                text: qsTr("Show Log")
                width: parent.width
                onClicked: {
                    rootTop.showMessageLog(true);
                    itemDrawer.close();
                }
            }

            ItemDelegate {
                text: qsTr("Hide Log")
                width: parent.width
                onClicked: {
                    rootTop.showMessageLog(false);
                    itemDrawer.close();
                }
            }

            ItemDelegate {
                text: qsTr("Exit")
                width: parent.width
                onClicked: {
                    Qt.quit();
                }
            }
        }

        Item {
            id: boxVersion
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 50

            HorizontalLine {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top

            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.verticalCenter
                text: programVersion
                font.pixelSize: 14
            }
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.verticalCenter
                text: gitHash
            }
        }
    }

    //==========================================================================
    // Main area
    //==========================================================================
    Item {
        anchors.fill: parent
        anchors.margins: 5

        Column {
            id: boxInfo
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 10

            // Port Info
            Item {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10
                height: 50

                Column {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.margins: 10
                    spacing: 5

                    LabeledText {
                        labelText: "Current Port:"
                        text: rootApp.selectedPort.name
                    }
                    LabeledText {
                        labelText: "Device:"
                        text: rootPage.deviceName
                    }

                }

                Button {
                    id: buttonOpenPort
                    anchors.right: buttonClosePort.left
                    anchors.margins: 10
                    text: qsTr("Start")
                    enabled: !portOpened
                    onClicked: {
                        buttonOpenPort.focus = true;
                        itemStatistic.clearFeedBuffer ();
                        if (rootApp.selectedPort) {
                            connectSensor ();
                        }
                    }
                }
                Button {
                    id: buttonClosePort
                    anchors.right: parent.right
                    anchors.margins: 10
                    text: qsTr("Stop")
                    enabled: portOpened
                    onClicked: {
                        itemUsbSensor.close();
                        itemStatistic.clearFeedBuffer ();
                        rootApp.appendMessageToLog (rootApp.selectedPort.name + " closed.");
                        portOpened = false;
                        deviceName = " ";
                    }
                }
            }

            HorizontalLine {
            }

            // Reading
            Loader {
                id: loaderReading
                anchors.left: parent.left
                anchors.right: parent.right
                height: 40

                // Statistic button
                ToolButton {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.margins: 5
                    icon.source: "../../assets/database.svg"
                    onClicked: {
                        navStack.push (pageStatistic);
                        rootTop.showMessageLog (false);
                    }
                }
            }

            // Horizontal Line
            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                height: 2
                color: Material.foreground
            }

        }

        // Chart
        Loader {
            id: loaderCharting
            anchors.top: boxInfo.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            onLoaded: {
                loaderCharting.item.clearData ();
            }

        }
    }

    //==========================================================================
    // Run on creation
    //==========================================================================
    Component.onCompleted: {
        rootApp.appendMessageToLog (objectName + " created. ");
    }

    //==========================================================================
    //==========================================================================
    Timer {
        id: timerUpdateProcess
        repeat: true
        interval: 500
        running: rootPage.portOpened
        triggeredOnStart: true

        property int lastUpdateTime: -1

        onRunningChanged: {
            lastUpdateTime = -1;
        }

        onTriggered: {
            var now = new Date();
            var timestamp = parseInt(now.getTime() / 1000);

            // Check crossing 5 second
            if ((timestamp % 5) != 0)
                return;
            if ((timestamp / 5) == (lastUpdateTime / 5)) {
                return;
            }

            lastUpdateTime = timestamp;
            console.log ("Current s=" + timestamp);

            var result;
            if (itemUsbSensor.update ()) {
                try {
                    result = JSON.parse (itemUsbSensor.result);
                    console.log ("Result:" + JSON.stringify(result));

                    // Convert Pa to hPa
                    if (result.P !== undefined) {
                        result.P /= 100;
                    }
                }
                catch (e){
                    console.log ("JSON parse error.");
                }

            }
            else {
                clearReading();
                itemUsbSensor.close();
                portOpened = false;
                deviceName = " ";
                rootApp.showSystemMessage ("ERROR", itemUsbSensor.lastErrorMessage);
            }

            if (loaderReading.status == Loader.Ready) {
                loaderReading.item.update (result);
            }
            if (loaderCharting.status == Loader.Ready) {
                loaderCharting.item.rollData (result);
            }

            if (result.T !== undefined) {
                itemStatistic.feedData (timestamp, "temperature", result.T);
            }
            if (result.H !== undefined) {
                itemStatistic.feedData (timestamp, "humidity", result.H);
            }
            if (result.P !== undefined) {
                itemStatistic.feedData (timestamp, "pressure", result.P);
            }
            if (result.TVOC !== undefined) {
                itemStatistic.feedData (timestamp, "tvoc", result.TVOC);
            }
            if (result.CO2eq !== undefined) {
                itemStatistic.feedData (timestamp, "co2eq", result.CO2eq);
            }


            itemStatistic.feedData (timestamp);

//            itemChart.rollData (temperature, humidity, pressure);
//            itemStatistic.feedData_PA (timestamp, result.T, result.H, result.P);
        }
    }

    //==========================================================================
    // Library
    //==========================================================================
    LocalLib.DelayFunc {
        id: itemDelayFunc
    }

}

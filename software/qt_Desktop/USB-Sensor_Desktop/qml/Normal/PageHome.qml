import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.3
import QtCharts 2.2

import "../LocalLib" as LocalLib

Page {
    id: rootPage
    title: qsTr("page1")

    //==========================================================================
    // Properties
    //==========================================================================
    property bool portOpened: false
    property string deviceName: " "
    property real temperature: 0
    property real humidity: 0
    property real pressure: 0
    property bool hasTemperature: true
    property bool hasHumidity: true
    property bool hasPressure: true
    property bool sendToClipboard: false

    property var arrayTemperature: []

    //==========================================================================
    // Functions
    //==========================================================================
    function clearReading () {
        temperature = Number.NaN;
        humidity = Number.NaN;
        pressure = Number.NaN;
        hasTemperature = true;
        hasHumidity = true;
        hasPressure = true;
    }


    //==========================================================================
    // ToolBar
    //==========================================================================
    header: TitleBar {
        titleText: rootApp.title

        leftLogoVisible: true
        leftLogoIconSource: "qrc:/assets/app_icon.png"
        onLeftButtonClicked: drawer.open();

        rightButtonVisible: true
        rightButtonEnabled: !rootPage.portOpened
        rightButtonIconSource: "qrc:/assets/settings-gears.svg"
        onRightButtonClicked: navStack.push (pageConfig);
    }

    Drawer {
        id: drawer
        width: rootPage.width * 0.20
        height: rootPage.height

        background: Rectangle {
            anchors.fill: parent
            border.width: 1
            border.color: "black"
        }

        Column {
            anchors.fill: parent

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
                text: qsTr("Exit")
                width: parent.width
                onClicked: {
                    Qt.quit();
                }
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
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 10

            // Port Info
            Item {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10
                height: 95

                Grid {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.margins: 10
                    columns: 2
                    spacing: 5

                    Label {
                        width: 120
                        horizontalAlignment: Text.AlignRight
                        text: "Current Port:"
                    }
                    Label {
                        text: rootApp.selectedPort.name
                    }
                    Label {
                        width: 120
                        horizontalAlignment: Text.AlignRight
                        text: "Device: "
                    }
                    Label {
                        text: rootPage.deviceName
                    }
                    Label {
                        text: " "
                    }

                    CheckBox  {
                        text: "Send reading to clipboard"
                        checked: rootPage.sendToClipboard
                        onCheckedChanged: {
                            rootPage.sendToClipboard = checked;
                        }
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
                        if (rootApp.selectedPort) {
                            if (itemUsbSensor.open (rootApp.selectedPort.name)) {
                                portOpened = true;
                                deviceName = itemUsbSensor.deviceId + " " + itemUsbSensor.deviceVersion;
                                hasTemperature = itemUsbSensor.hasTemperature;
                                hasHumidity = itemUsbSensor.hasHumudity;
                                hasPressure = itemUsbSensor.hasPressure;
                            }
                            else {
                                rootApp.showSystemMessage ("ERROR", itemUsbSensor.lastErrorMessage);
                                deviceName = " ";
                            }
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
                        rootApp.appendMessageToLog (rootApp.selectedPort.name + " closed.");
                        portOpened = false;
                        deviceName = " ";
                        clearReading();
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

            // Reading
            Grid {
                id: boxReading
                anchors.horizontalCenter: parent.horizontalCenter
                height: 40
                columns: 2
                spacing: 10

                Row {
                    width: 200
                    height: 15
                    spacing: 5
                    visible: hasTemperature

                    Rectangle {
                        height: 12
                        width: 12
                        border.width: 1
                        color: "gold"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Label {
                        anchors.verticalCenter: parent.verticalCenter
                        text: "Temperature: " + temperature + " °C"
                    }
                }

                Row {
                    width: 200
                    height: 15
                    spacing: 5
                    visible: hasHumidity

                    Rectangle {
                        height: 12
                        width: 12
                        border.width: 1
                        color: "cyan"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Label {
                        anchors.verticalCenter: parent.verticalCenter
                        text: "Humidity: " + humidity + " %RH"
                    }
                }
                Row {
                    width: 200
                    height: 15
                    spacing: 5
                    visible: hasPressure

                    Rectangle {
                        height: 12
                        width: 12
                        border.width: 1
                        color: "greenyellow"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Label {
                        anchors.verticalCenter: parent.verticalCenter
                        text: "Pressure: " + pressure + " hPa"
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

            // Chart
            RollingChart {
                id: chart
                anchors.left: parent.left
                anchors.right: parent.right
                height: 240

                lineTemperature.color: "gold"
                lineTemperature.visible: hasTemperature
                lineHumidity.color: "cyan"
                lineHumidity.visible: hasHumidity
                linePressure.color: "greenyellow"
                linePressure.visible: hasPressure
            }
        }
    }

    //==========================================================================
    // Run on creation
    //==========================================================================
    Component.onCompleted: {
        rootApp.appendMessageToLog (objectName + " created. ");
        clearReading();
        chart.clearData();
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

            if (itemUsbSensor.update ()) {
                temperature = itemUsbSensor.temperature;
                humidity = itemUsbSensor.humidity;
                pressure = itemUsbSensor.pressure / 100;

                chart.rollData (temperature, humidity, pressure);

                if (sendToClipboard) {
                    console.log ("Send to clipboard.");
                    var str_output = temperature.toFixed(2) + ", " + humidity.toFixed(2)
                                        + ", " + pressure.toFixed(2) + ", " + timestamp;
                    itemUsbSensor.sendToClipboard (str_output);
                }
            }
            else {
                clearReading();
                itemUsbSensor.close();
                portOpened = false;
                deviceName = " ";
                rootApp.showSystemMessage ("ERROR", itemUsbSensor.lastErrorMessage);
            }
        }
    }

    //==========================================================================
    // Library
    //==========================================================================
    LocalLib.DelayFunc {
        id: itemDelayFunc
    }

}

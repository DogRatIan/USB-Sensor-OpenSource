import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.3

//import com.magnaconcept.qml.FakeILock 1.0
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
                height: 50

                Row {
                    spacing: 10
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.margins: 10

                    Label {
                        text: "Current Port:"
                    }
                    Label {
                        text: selectedPort.name;
                    }
                }
                Label {
                    anchors.bottom: parent.bottom
                    text: "Device: " + deviceName
                }

                Button {
                    id: buttonOpenPort
                    anchors.right: buttonClosePort.left
                    anchors.margins: 10
                    text: qsTr("Start")
                    enabled: !portOpened
                    onClicked: {
                        buttonOpenPort.focus = true;
                        if (selectedPort) {
                            if (itemUsbSensor.open (selectedPort.name)) {
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
                        rootApp.appendMessageToLog (selectedPort.name + " closed.");
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
//                anchors.left: parent.left
//                anchors.right: parent.right
                height: 40
                columns: 2
                spacing: 10

                Label {
                    visible: hasTemperature
                    width: 200
                    text: "Temperature: " + temperature + " °C"
                }
                Label {
                    visible: hasHumidity
                    width: 200
                    text: "Humidity: " + humidity + " %RH"
                }
                Label {
                    visible: hasPressure
                    width: 200
                    text: "Pressure: " + pressure + " hPa"
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
    }

    //==========================================================================
    // Run on creation
    //==========================================================================
    Component.onCompleted: {
        rootApp.appendMessageToLog (objectName + " created. ");
        clearReading();

    }

    //==========================================================================
    //==========================================================================
    Timer {
        id: timerUpdateProcess
        repeat: true
        interval: 5000
        running: rootPage.portOpened
        triggeredOnStart: true
        onTriggered: {
            if (itemUsbSensor.update ()) {
                temperature = itemUsbSensor.temperature;
                humidity = itemUsbSensor.humidity;
                pressure = itemUsbSensor.pressure / 100;
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

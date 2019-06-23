import QtQuick 2.11
import QtQuick.Controls 2.4

Item {
    anchors.fill: parent

    //==========================================================================
    //==========================================================================
    property real temperature: 0
    property real humidity: 0
    property real pressure: 0

    //==========================================================================
    // Functions
    //==========================================================================
    function update (aResult) {
        console.log ("Reading_PA update()");

        if (aResult === undefined) {
            temperature = 0;
            humidity = 0;
            pressure = 0;
            return;
        }

        if (aResult.T === undefined) {
            boxTemperature.visible = false;
            temperature = 0;
        }
        else {
            boxTemperature.visible = true;
            temperature = aResult.T;
        }
        if (aResult.H === undefined) {
            boxHumidity.visible = false;
            humidity = 0;
        }
        else {
            boxHumidity.visible = true;
            humidity = aResult.H;
        }
        if (aResult.P === undefined) {
            boxPressure.visible = false;
            pressure = 0;
        }
        else {
            boxPressure.visible = true;
            pressure = aResult.P;
        }
    }

    //==========================================================================
    //==========================================================================
    Grid {
        anchors.horizontalCenter: parent.horizontalCenter
        columns: 2
        spacing: 10

        Row {
            id: boxTemperature
            width: 200
            height: 15
            spacing: 5

            Rectangle {
                height: 12
                width: 12
                border.width: 1
                color: "gold"
                anchors.verticalCenter: parent.verticalCenter
            }
            Label {
                anchors.verticalCenter: parent.verticalCenter
                text: "Temperature: " + temperature.toFixed(2) + " °C"
            }
        }

        Row {
            id: boxHumidity
            width: 200
            height: 15
            spacing: 5

            Rectangle {
                height: 12
                width: 12
                border.width: 1
                color: "cyan"
                anchors.verticalCenter: parent.verticalCenter
            }
            Label {
                anchors.verticalCenter: parent.verticalCenter
                text: "Humidity: " + humidity.toFixed(2) + " %RH"
            }
        }

        Row {
            id: boxPressure
            width: 200
            height: 15
            spacing: 5
            visible: false

            Rectangle {
                height: 12
                width: 12
                border.width: 1
                color: "greenyellow"
                anchors.verticalCenter: parent.verticalCenter
            }
            Label {
                anchors.verticalCenter: parent.verticalCenter
                text: "Pressure: " + pressure.toFixed(4) + " hPa"
            }
        }
    }
}

import QtQuick 2.11
import QtQuick.Controls 2.4

Item {
    anchors.fill: parent

    //==========================================================================
    //==========================================================================
    property real co2eq: 0
    property real tvoc: 0

    //==========================================================================
    // Functions
    //==========================================================================
    function update (aResult) {
        console.log ("Reading_VOC update()");

        if (aResult === undefined) {
            co2eq = 0;
            tvoc = 0;
            return;
        }

        if (aResult.CO2eq === undefined) {
            boxCO2.visible = false;
            co2eq = 0;
        }
        else {
            boxCO2.visible = true;
            co2eq = aResult.CO2eq;
        }

        if (aResult.TVOC === undefined) {
            boxTVOC.visible = false;
            tvoc = 0;
        }
        else {
            boxTVOC.visible = true;
            tvoc = aResult.TVOC;
        }
    }

    //==========================================================================
    //==========================================================================
    Grid {
        anchors.horizontalCenter: parent.horizontalCenter
        columns: 2
        spacing: 10

        Row {
            id: boxTVOC
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
                text: "TVOC: " + tvoc.toFixed(2) + " ppb"
            }
        }

        Row {
            id: boxCO2
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
                text: "CO²eq: " + co2eq.toFixed(2) + " ppm"
            }
        }
    }
}

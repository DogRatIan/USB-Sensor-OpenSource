import QtQuick 2.11
import QtQuick.Controls 2.4
import QtCharts 2.2

//
ChartView {
    antialiasing: true
    theme: ChartView.ChartThemeDark
    legend.visible: false

    property alias lineTVOC: lineTVOC
    property alias lineCO2: lineCO2

    //==========================================================================
    // Functions
    //==========================================================================
    function clearData () {
        lineTVOC.clear();
        lineCO2.clear();
        for (var x = -300; x <= 0; x += 5) {
            lineTVOC.append (x, 0);
            lineCO2.append (x, 0);
        }
    }

    function rollData (aResult) {
        // Default is 0 (for NAN)
        var tvoc = 0;
        var co2eq = 0;
        var max_value = 0;

        if (aResult !== undefined) {
            if (aResult.TVOC !== undefined) {
                tvoc = aResult.TVOC;
            }
            if (aResult.CO2eq !== undefined) {
                co2eq = aResult.CO2eq;
            }
        }
        console.log ("Charting_VOC rollData " + tvoc + ", " + co2eq);
        if (tvoc > co2eq)
            max_value = tvoc;
        else
            max_value = co2eq;


        var point1;
        var point2;
        for (var i = 0; i < lineTVOC.count - 1; i ++) {
            point1 = lineTVOC.at (i);
            point2 = lineTVOC.at (i + 1);
            point2.x = point1.x;
            lineTVOC.replace (point1.x, point1.y, point2.x, point2.y);
            if (point2.y > max_value)
                max_value = point2.y;

            point1 = lineCO2.at (i);
            point2 = lineCO2.at (i + 1);
            point2.x = point1.x;
            lineCO2.replace (point1.x, point1.y, point2.x, point2.y);
            if (point2.y > max_value)
                max_value = point2.y;
        }

        point1 = lineTVOC.at (lineTVOC.count - 1);
        lineTVOC.replace (point1.x, point1.y, 0, tvoc);
        point1 = lineCO2.at (lineCO2.count - 1);
        lineCO2.replace (point1.x, point1.y, 0, co2eq);

        var axis_max = [750000, 50000, 37500, 25000, 10000, 1000, 500, 0];
        for (i = 0; i < axis_max.length - 1; i ++) {
            if (max_value >= axis_max[i + 1]) {
                break;
            }
        }
        axisY.max = axis_max[i];
    }

    //==========================================================================
    //==========================================================================
    ValueAxis {
        id: axisY
        min: 0
        max: 60000
        tickCount: 6
    }

    ValueAxis {
        id: axisX
        min: -300
        max: 0
        minorTickCount: 5
        tickCount: 11
        labelFormat: "%ds"
    }
    LineSeries {
        id: lineTVOC
        axisX: axisX
        axisY: axisY
        width: 2
        color: "gold"
    }
    LineSeries {
        id: lineCO2
        axisX: axisX
        axisY: axisY
        width: 2
        color: "cyan"
    }
}

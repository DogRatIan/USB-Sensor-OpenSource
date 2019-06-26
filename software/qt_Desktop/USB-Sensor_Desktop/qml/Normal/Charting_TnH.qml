import QtQuick 2.11
import QtQuick.Controls 2.4
import QtCharts 2.2

//
ChartView {
    antialiasing: true
    theme: ChartView.ChartThemeDark
    legend.visible: false

    property alias lineTemperature: lineTemperature
    property alias lineHumidity: lineHumidity

    //==========================================================================
    // Functions
    //==========================================================================
    function clearData () {
        lineTemperature.clear();
        lineHumidity.clear();
        for (var x = -300; x <= 0; x += 5) {
            lineTemperature.append (x, 0);
            lineHumidity.append (x, 0);
        }
    }

    function rollData (aResult) {
        // Default is 0 (for NAN)
        var temperature = 0;
        var humidity = 0;

        if (aResult !== undefined) {
            if (aResult.T !== undefined) {
                temperature = aResult.T;
            }
            if (aResult.H !== undefined) {
                humidity = aResult.H;
            }
        }
        console.log ("Charting_TnH rollData " + temperature + ", " + humidity);


        var point1;
        var point2;
        for (var i = 0; i < lineTemperature.count - 1; i ++) {
            point1 = lineTemperature.at (i);
            point2 = lineTemperature.at (i + 1);
            point2.x = point1.x;
            lineTemperature.replace (point1.x, point1.y, point2.x, point2.y);

            point1 = lineHumidity.at (i);
            point2 = lineHumidity.at (i + 1);
            point2.x = point1.x;
            lineHumidity.replace (point1.x, point1.y, point2.x, point2.y);
        }

        point1 = lineTemperature.at (lineTemperature.count - 1);
        lineTemperature.replace (point1.x, point1.y, 0, temperature);
        point1 = lineHumidity.at (lineHumidity.count - 1);
        lineHumidity.replace (point1.x, point1.y, 0, humidity);
    }

    //==========================================================================
    //==========================================================================
    ValueAxis {
        id: axisY
        min: -20
        max: 100
        tickCount: 7
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
        id: lineTemperature
        axisX: axisX
        axisY: axisY
        width: 2
        color: "gold"
    }
    LineSeries {
        id: lineHumidity
        axisX: axisX
        axisY: axisY
        width: 2
        color: "cyan"
    }
}

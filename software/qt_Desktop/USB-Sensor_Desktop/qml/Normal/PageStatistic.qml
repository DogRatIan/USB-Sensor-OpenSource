import QtQuick 2.11
import QtQuick.Controls 2.4

Page {
    id: rootPage
    title: qsTr("Statistic")

    //==========================================================================
    // Functions
    //==========================================================================
    function updateStatus () {
        if (itemStatistic.isReady) {
            textDatabaseStatus.text = "Ready";
        }
        else {
            textDatabaseStatus.text = "Not connected.";
        }
        textDatabaseFilename.text = itemStatistic.filename;

        var file_size =  Number (itemStatistic.fileSize);
        if (file_size < 1) {
            file_size *= 1024;
            textDatabaseFileSize.text = file_size.toFixed(2) + " KiB";
        }
        else {
            textDatabaseFileSize.text = file_size.toFixed(2) + " MiB";
        }

        if (itemStatistic.currentPeriodStart === itemStatistic.currentPeriodEnd) {
            // No averaging
            textAveragingPeriod.text = "Not running."
        }
        else {
            var d_start = new Date (itemStatistic.currentPeriodStart * 1000);
            var d_end = new Date (itemStatistic.currentPeriodEnd * 1000);

            textAveragingPeriod.text = d_start.toTimeString() + " to " + d_end.toTimeString();
        }
        textAvgDataCount.text = "Temp=" + itemStatistic.avgTemperatureCount
                                + ", Humi=" + itemStatistic.avgHumidityCount
                                + ", Pressure=" + itemStatistic.avgPressureCount;

        textLastSavedData.text = "Temp=" + Number(itemStatistic.lastTemperature).toFixed(2)
                                + ", Humi=" + Number(itemStatistic.lastHumidity).toFixed(2)
                                + ", Pressure=" + Number(itemStatistic.lastPressure).toFixed(2);
        if (parseInt(itemStatistic.lastTimestamp) === 0) {
            textLastSavedTimestamp.text = " ";
        }
        else {
            var d_last = new Date (itemStatistic.lastTimestamp * 1000);
            textLastSavedTimestamp.text = d_last.toISOString();
        }
    }

    function onFeedDataFinished() {
        console.log ("onFeedDataFinished");
        updateStatus ();
    }

    //==========================================================================
    // ToolBar
    //==========================================================================
    header: TitleBar {
        titleText: rootPage.title

        leftButtonVisible: true
        leftButtonIconSource: "qrc:/assets/back-arrow.svg"
        onLeftButtonClicked: navStack.pop ();
    }

    //==========================================================================
    // Main area
    //==========================================================================
    Item {
        anchors.fill: parent
        anchors.margins: 10

        Column {
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 10

            LabeledText {
                id: textDatabaseStatus
                labelText: "DB status:"
            }
            LabeledText {
                id: textDatabaseFilename
                labelText: "DB filename:"
            }
            LabeledText {
                id: textDatabaseFileSize
                labelText: "DB file size:"
            }

            HorizontalLine {
            }

            LabeledText {
                id: textAveragingPeriod
                labelText: "Averaging Period:"
            }
            LabeledText {
                id: textAvgDataCount
                labelText: "Data count:"
            }
            LabeledText {
                id: textLastSavedData
                labelText: "Last saved:"
            }
            LabeledText {
                id: textLastSavedTimestamp
                labelText: " "
            }
        }

    }

    //==========================================================================
    // Creation/Destruction
    //==========================================================================
    Component.onCompleted: {
        appendMessageToLog (objectName + " created.");
        updateStatus ();
        itemStatistic.feedFinishedCallback = rootPage.onFeedDataFinished;
    }
    Component.onDestruction: {
        itemStatistic.feedFinishedCallback = undefined;
    }
}

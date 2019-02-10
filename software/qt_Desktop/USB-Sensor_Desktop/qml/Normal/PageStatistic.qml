import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2

Page {
    id: rootPage
    title: qsTr("Statistic")

    property var targetExportPath

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

    function onExportSelected (aDest) {
        console.log ("onExportSelected: " + aDest);
        targetExportPath = aDest;
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

            HorizontalLine {
            }

            Item {
                anchors.left: parent.left
                anchors.right: parent.right
                height: boxRemoveOldData.height

                Row {
                    id: boxRemoveOldData
                    spacing: 10

                    Label {
                        anchors.verticalCenter: parent.verticalCenter
                        width: 150
                        text: "Remove old data:"
                        font.bold: true
                        horizontalAlignment: Text.AlignRight
                    }
                    ComboBox {
                        id: comboBoxRemoveDataLen
                        anchors.verticalCenter: parent.verticalCenter
                        width: 200
                        editable: false
                        textRole: "text"
                        model: ListModel {
                            id: comboBoxRemoveDataModel
                            ListElement { length: -1; text: "All" }
                            ListElement { length: 24; text: "1 days ago" }
                            ListElement { length: 168; text: "7 days ago" }
                        }
                    }
                }
                Button {
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Proceed"
                    onClicked: {
                        var selected = comboBoxRemoveDataModel.get (comboBoxRemoveDataLen.currentIndex);
                        console.log ("selected=" + selected.length);
                        itemStatistic.removeOldData (selected.length);
                    }
                }
            }

            HorizontalLine {
            }

            LabeledText {
                labelText: "Export data:"
                text: targetExportPath
            }

            Row {
                anchors.right: parent.right
                spacing: 10
                Button {
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Browse"
                    onClicked: {
                        dialogLoader.open ();
                    }
                }
                Button {
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Export"
                    onClicked: {
                        if (itemConfig.getStringData ("lastExportPath") !== targetExportPath) {
                            itemConfig.setStringData ("lastExportPath", targetExportPath);
                            itemConfig.save ();
                        }
                        if (!itemStatistic.exportData (targetExportPath)) {
                        }
                    }
                }
            }

            HorizontalLine {
            }

        }
    }
    Loader {
        id: dialogLoader
        anchors.fill: parent
        focus: true

        function open () {
            if (dialogLoader.status == Loader.Ready) {
                dialogLoader.item.open ();
            }
            else {
                dialogLoader.source = "DialogSelectExport.qml"
            }
            if (targetExportPath.length > 0) {
                dialogLoader.item.folder = "file:///" + targetExportPath;
            }
        }

        onLoaded: {
            dialogLoader.item.callbackFunc = rootPage.onExportSelected;
            dialogLoader.item.open ();
        }
    }


    //==========================================================================
    // Creation/Destruction
    //==========================================================================
    Component.onCompleted: {
        appendMessageToLog (objectName + " created.");
        updateStatus ();
        itemStatistic.feedFinishedCallback = rootPage.onFeedDataFinished;
        targetExportPath = itemConfig.getStringData ("lastExportPath");
    }
    Component.onDestruction: {
        itemStatistic.feedFinishedCallback = undefined;
    }
}

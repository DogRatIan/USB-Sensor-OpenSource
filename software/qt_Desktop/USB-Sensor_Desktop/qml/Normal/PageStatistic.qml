import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2

Page {
    id: rootPage
    title: qsTr("Statistic")

    property var targetExportPath: ""

    //==========================================================================
    // Functions
    //==========================================================================
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
        leftButtonIconSource: "../../assets/back-arrow.svg"
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
                text: dbStatus
            }
            LabeledText {
                id: textDatabaseFilename
                labelText: "DB filename:"
                text: dbFilename
            }
            LabeledText {
                id: textDatabaseFileSize
                labelText: "DB file size:"
                text: dbFileSize
            }

            HorizontalLine {
            }

            LabeledText {
                id: textAveragingPeriod
                labelText: "Averaging Period:"
                text: averagingPeriod
            }
            LabeledText {
                id: textAvgDataCount
                labelText: "Data count:"
                text: lastDataCount
            }
            LabeledText {
                id: textLastSavedData
                labelText: "Last saved:"
                text: lastSavedValues
            }
            LabeledText {
                id: textLastSavedTimestamp
                labelText: " "
                text: lastSavedTime
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
                        if (itemStatistic.removeOldData (selected.length)) {
                            rootApp.showSystemMessage ("INFO", "Statistic data removed.");
                        }
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
                        if (targetExportPath.length === 0) {
                            rootApp.showSystemMessage ("ERROR", "Please select path for export.");
                            return;
                        }

                        if (itemConfig.getStringData ("lastExportPath") !== targetExportPath) {
                            itemConfig.setStringData ("lastExportPath", targetExportPath);
                            itemConfig.save ();
                        }
                        if (itemStatistic.exportData (targetExportPath)) {
                            rootApp.showSystemMessage ("INFO", "Export done.");
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
        rootApp.updateDbInfo ();
        targetExportPath = itemConfig.getStringData ("lastExportPath");
    }
    Component.onDestruction: {
    }
}

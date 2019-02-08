import QtQuick 2.11
import QtQuick.Controls 2.4

Page {
    id: rootPage
    title: qsTr("Statistic")

    //==========================================================================
    // Functions
    //==========================================================================

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

    }

    //==========================================================================
    // Creation/Destruction
    //==========================================================================
    Component.onCompleted: {
        appendMessageToLog (objectName + " created.");
    }
}

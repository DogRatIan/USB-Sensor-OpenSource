import QtQuick 2.11
import QtQuick.Controls 2.4

//
ToolBar {
    id: rootItem

    property string titleText
    property alias rightButtonVisible: buttonRight.visible
    property alias rightButtonEnabled: buttonRight.enabled
    property alias rightButtonIconSource: buttonRight.icon.source

    property alias leftButtonVisible: buttonLeft.visible
    property alias leftButtonEnabled: buttonLeft.enabled
    property alias leftButtonIconSource: buttonLeft.icon.source

    property alias leftLogoVisible: iconLeft.visible
    property alias leftLogoIconSource: iconLeft.source

    property var clickPos

    signal rightButtonClicked ()
    signal leftButtonClicked ()

    // Left button
    ToolButton {
        id: buttonLeft
        visible: false
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        onClicked: rootItem.leftButtonClicked ();
    }

    Image {
        id: iconLeft
        visible: false
        anchors.fill: buttonLeft
        anchors.margins: 5
        fillMode: Image.PreserveAspectFit
    }
    MouseArea {
        anchors.fill: iconLeft
        onClicked: rootItem.leftButtonClicked ();
    }

    // Title text
    Label {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        text: rootItem.titleText
    }

    // Right button
    ToolButton {
        id: buttonRight
        visible: false
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        onClicked: rootItem.rightButtonClicked ();
    }

    // For draging whole window
    MouseArea {
        anchors.left: buttonLeft.right
        anchors.right: buttonRight.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        onPressed: {
            clickPos  = Qt.point(mouse.x,mouse.y)
        }
        onPositionChanged: {
            var delta = Qt.point(mouse.x-clickPos.x, mouse.y-clickPos.y)
            rootApp.x += delta.x;
            rootApp.y += delta.y;
        }
    }
}

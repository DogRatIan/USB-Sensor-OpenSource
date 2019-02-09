import QtQuick 2.11
import QtQuick.Controls 2.4

// Horizontal Line
Row {
    property alias labelText: itemLabel.text
    property alias text: itemText.text

    height: itemLabel.height
    spacing: 10
    Label {
        id: itemLabel
        width: 150
        horizontalAlignment: Text.AlignRight
        font.bold: true
        text: "?? :"
    }
    Label {
        id: itemText
        text: "?"
    }
}

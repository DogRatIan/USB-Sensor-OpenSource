import QtQuick 2.11
import QtQuick.Dialogs 1.2

FileDialog {
    id: dialogSelectExport
    visible: false
    title: "Please choose the destination"
    selectExisting: true
    selectFolder: true
    selectMultiple: false
    folder: shortcuts.home

    property var callbackFunc;


    function urlToPath (aUrlString) {
        var path = aUrlString.toString().replace (/^(file:\/{3})|(qrc:\/{2})|(http:\/{2})/,"");
        return decodeURIComponent(path);
    }

    onAccepted: {
        console.log("You chose: " + dialogSelectExport.fileUrl)
        close ();
        if (typeof (callbackFunc) === "function") {
            callbackFunc (urlToPath (dialogSelectExport.fileUrl));
        }
    }
    onRejected: {
        console.log("Canceled")
        close ();
        if (typeof (callbackFunc) === "function") {
            callbackFunc ("");
        }
    }
}

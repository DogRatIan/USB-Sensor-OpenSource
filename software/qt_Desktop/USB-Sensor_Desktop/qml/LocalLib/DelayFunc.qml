import QtQuick 2.0
import QtQuick.LocalStorage 2.0

Item {

    property var delayedCallbackFunc

    // Function - Delay and Call back
    function set (aDelayTime, aCallback) {
        if (!timerDelayedFunc.running) {
            delayedCallbackFunc = aCallback;
            timerDelayedFunc.interval = aDelayTime;
            timerDelayedFunc.repeat = false;
            timerDelayedFunc.start();
        }
    }
    function forceSet (aDelayTime, aCallback) {
        timerDelayedFunc.stop ();
        delayedCallbackFunc = aCallback;
        timerDelayedFunc.interval = aDelayTime;
        timerDelayedFunc.repeat = false;
        timerDelayedFunc.start();
    }

    //==========================================================================
    // Timer
    //==========================================================================
    Timer {
        id: timerDelayedFunc
        onTriggered: {
            if (delayedCallbackFunc !== undefined) {
                delayedCallbackFunc ();
            }
        }
    }
}

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Dialogs 1.3

Rectangle {
    id:settingsMainItem
    width: 480
    height: 480
    color: "#5c5c5c"
    anchors.centerIn: parent


    Button {
        id: leftBtn
        x: 30
        y: 51
        width: 102
        height: 27
        text: qsTr("Left")
        background: Rectangle {
            radius: 4
            color: "#dbdbdb"
        }
    }

    Label {
        id: signalLabel
        x: 30
        y: 25
        width: 102
        height: 20
        text: qsTr("Turn Signals")
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 12
    }

    Button {
        id: rightBtn
        x: 30
        y: 97
        width: 102
        height: 27
        text: qsTr("Right")
        background: Rectangle {
            color: "#dbdbdb"
            radius: 4
        }
    }

    Button {
        id: hazardBtn
        x: 30
        y: 142
        width: 102
        height: 27
        text: qsTr("Hazards")
        background: Rectangle {
            color: "#dbdbdb"
            radius: 4
        }
    }

    Button {
        id: signalOffBtn
        x: 30
        y: 187
        width: 102
        height: 27
        text: qsTr("Off")
        background: Rectangle {
            color: "#dbdbdb"
            radius: 4
        }
    }

    Label {
        id: lockLabel
        x: 189
        y: 25
        width: 102
        height: 20
        text: qsTr("Locks")
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 12
    }

    Button {
        id: unlockBtn
        x: 189
        y: 97
        width: 102
        height: 27
        text: qsTr("Unlock")
        background: Rectangle {
            color: "#dbdbdb"
            radius: 4
        }
    }

    Button {
        id: lockBtn
        x: 189
        y: 51
        width: 102
        height: 27
        text: qsTr("Lock")
        background: Rectangle {
            color: "#dbdbdb"
            radius: 4
        }
    }

    Button {
        id: exitBtn
        x: 409
        y: 8
        width: 63
        height: 17
        text: qsTr("Exit")
        onClicked: {
            settingsMainItem.destroy()
        }
    }

    Button {
        id: resetTripA
        objectName: "resetTripABtn"
        x: 341
        y: 51
        width: 102
        height: 27
        text: qsTr("Reset Trip A")
        background: Rectangle {
            color: "#dbdbdb"
            radius: 4
        }
        onClicked: {
            gauge.resetTrip("tripA");
        }
    }

    Label {
        id: trip
        x: 341
        y: 25
        width: 102
        height: 20
        text: qsTr("Trip")
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 12
    }

    Button {
        id: resetTripB
        objectName: "resetTripBBtn"
        x: 341
        y: 97
        width: 102
        height: 27
        text: qsTr("Reset Trip B")
        background: Rectangle {
            color: "#dbdbdb"
            radius: 4
        }
        onClicked: {
            gauge.resetTrip("tripB");
        }
    }
    Button {
        id: closeProgram
        objectName: "killProgramBtn"
        x: 341
        y: 400
        width: 102
        height: 27
        text: qsTr("Close Program")
        background: Rectangle {
            color: "#dbdbdb"
            radius: 4
        }
        onClicked: {
            Qt.quit();
        }
    }

    Label {
        id: shiftLabel
        x: 30
        y: 238
        width: 102
        height: 20
        text: qsTr("Shift Light")
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 12
    }

    TextInput {
        id: shiftThreshInput
        x: 72
        y: 264
        width: 60
        height: 20
        text: gauge.getShiftThreshold()
        font.pixelSize: 12
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        cursorVisible: false
        maximumLength: 4
        mouseSelectionMode: TextInput.SelectCharacters
        MouseArea{
            anchors.fill: parent;
            cursorShape: Qt.IBeamCursor;
            onClicked: mouse.accepted = false;
            onPressed: mouse.accepted = false;
            onReleased: mouse.accepted = false;
            propagateComposedEvents: true;
            onPositionChanged: mouse.accepted = false;
            onPressAndHold: mouse.accepted = false;
        }
        onAccepted:{
            gauge.setShiftThreshold(shiftThreshInput.text);
            cursorVisible: false;
            focus: false;
            activeFocus: false;
            settingsMainItem.forceActiveFocus();
            nextItemInFocusChain();

            Qt.BlankCursor;
        }

    }

    Label {
        id: label
        x: 25
        y: 266
        width: 48
        height: 16
        text: qsTr("RPM")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    Label {
        id: miscLabel
        x: 341
        y: 161
        width: 102
        height: 20
        text: qsTr("Misc")
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 12
    }

    Button {
        id: toggleThrottleBtn
        x: 341
        y: 187
        width: 102
        height: 27
        text: qsTr("Accel Bar Toggle")
        objectName: "resetTripBBtn"
        background: Rectangle {
            color: "#dbdbdb"
            radius: 4
        }
        onClicked:{
            gauge.toggleThrottleBar();
        }
    }
    Button {
        id: openTestFileBtn
        x: 341
        y: 187
        width: 102
        height: 27
        text: qsTr("Open Log")
        objectName: "openLogBtn"
        background: Rectangle {
            color: "#dbdbdb"
            radius: 4
        }
        onClicked:{
            fileDialog.open();
        }
    }

    FileDialog {
            id: fileDialog
            title: "Select a file"
            onAccepted: {
                console.log("Selected file: " + fileDialog.fileUrl)
                tester.testFileSelected(fileDialog.fileUrl)
                // Pass the file URL to your Qt program here
            }
            onRejected: console.log("File selection cancelled")
    }



}

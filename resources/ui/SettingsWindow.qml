import QtQuick 2.15
import QtQuick.Controls 2.15

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
}

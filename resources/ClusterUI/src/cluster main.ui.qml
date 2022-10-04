

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    width: 1920
    height: 720
    color: "#21202c"

    Image {
        id: tach
        x: 0
        y: 0
        width: 620
        height: 720
        source: "../../_Tuning/Gauge Cluster/tach.svg"
        sourceSize.width: 620
        mipmap: true
        antialiasing: true
        rotation: 0
        transformOrigin: Item.Bottom
        fillMode: Image.PreserveAspectCrop

        Rectangle {
            id: tachneedle
            x: 260
            y: 60
            width: 100
            height: 600
            visible: true
            color: "#005d2d2d"
            rotation: -90

            Image {
                id: needlePic
                x: 0
                y: 51
                width: 100
                height: 285
                horizontalAlignment: Image.AlignHCenter
                verticalAlignment: Image.AlignTop
                source: "../../_Tuning/Gauge Cluster/needle.svg"
                rotation: 0
                transformOrigin: Item.Bottom
                sourceSize.width: 100
                sourceSize.height: 300
                fillMode: Image.PreserveAspectCrop
            }
        }
    }

    Image {
        id: speedo
        x: 1300
        width: 620
        height: 720
        visible: true
        source: "../../_Tuning/Gauge Cluster/speedo.svg"
        sourceSize.height: 720
        sourceSize.width: 620
        antialiasing: true
        mipmap: true
        fillMode: Image.PreserveAspectCrop

        Rectangle {
            id: speedoNeedle
            x: 260
            y: 60
            width: 100
            height: 600
            visible: true
            color: "#005d2d2d"
            rotation: -41
            Image {
                id: needlePic1
                x: 0
                y: 51
                width: 100
                height: 285
                horizontalAlignment: Image.AlignHCenter
                verticalAlignment: Image.AlignTop
                source: "../../_Tuning/Gauge Cluster/needle.svg"
                rotation: 0
                fillMode: Image.PreserveAspectCrop
                sourceSize.width: 100
                transformOrigin: Item.Bottom
                sourceSize.height: 300
            }
        }

        Rectangle {
            id: speedoNeedle1
            x: 170
            y: 200
            width: 100
            height: 500
            visible: true
            color: "#005d2d2d"
            scale: 1
            rotation: -90
            Image {
                id: needlePic2
                x: 0
                y: 120
                width: 100
                height: 130
                horizontalAlignment: Image.AlignHCenter
                verticalAlignment: Image.AlignTop
                source: "../../_Tuning/Gauge Cluster/needle.svg"
                rotation: 0
                sourceSize.width: 100
                fillMode: Image.PreserveAspectCrop
                transformOrigin: Item.Bottom
                sourceSize.height: 300
            }

            Rectangle {
                id: rectangle
                width: 100
                height: 1
                color: "#ffffff"
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }

    Rectangle {
        id: rectangle1
        x: 626
        y: 86
        width: 668
        height: 550
        color: "#21202c"

        Rectangle {
            id: textbox
            y: 298
            width: 500
            height: 250
            color: "#21202c"
            border.color: "#535353"
            border.width: 2
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 2
            anchors.horizontalCenter: parent.horizontalCenter

            TextArea {
                id: paramText
                x: 61
                y: -148
                width: 500
                height: 250
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                placeholderText: qsTr("")
            }
        }

        Rectangle {
            id: textbox1
            y: 298
            width: 500
            height: 250
            color: "#21202c"
            border.color: "#535353"
            border.width: 2
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.horizontalCenter: parent.horizontalCenter
            TextArea {
                id: frameText
                x: 61
                y: -148
                width: 500
                height: 250
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                placeholderText: qsTr("")
            }
        }
    }

    Button {
        id: button
        x: 706
        y: 32
        width: 118
        height: 48
        text: qsTr("Defs")
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.5}D{i:9;invisible:true}D{i:12}D{i:11}D{i:13}
}
##^##*/


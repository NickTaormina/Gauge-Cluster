

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window{
    id:root
    width:1920
    height: 720
    visible:true


Rectangle {
    width: 1920
    height: 720
    color: "#181719"

    Image {
        id: tach
        x: 0
        y: 0
        width: 620
        height: 720
        source: "file:///C:/Users/admin/OneDrive - University of Florida/Documents/_Tuning/Gauge Cluster/tach.png"
        sourceSize.width: 620
        mipmap: true
        antialiasing: true
        rotation: 0
        transformOrigin: Item.Bottom
        fillMode: Image.PreserveAspectCrop

        Rectangle {
            id: tachneedle
            objectName: "tachneedle"
            x: 260
            y: 60
            width: 100
            height: 600
            visible: true
            color: "#005d2d2d"
            rotation: 270



            Image {
                id: needlePic
                x: 0
                y: 51
                width: 100
                height: 285
                horizontalAlignment: Image.AlignHCenter
                verticalAlignment: Image.AlignTop
                source: "file:///C:/Users/admin/OneDrive - University of Florida/Documents/_Tuning/Gauge Cluster/needle.svg"
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
        source: "file:///C:/Users/admin/OneDrive - University of Florida/Documents/_Tuning/Gauge Cluster/speedo.png"
        sourceSize.height: 720
        sourceSize.width: 620
        antialiasing: true
        mipmap: true
        fillMode: Image.PreserveAspectCrop

        Rectangle {
            id: speedoNeedle
            objectName: "speedoneedle"
            x: 260
            y: 60
            width: 100
            height: 600
            visible: true
            color: "#005d2d2d"
            rotation: 300
            Image {
                id: needlePic1
                x: 0
                y: 51
                width: 100
                height: 285
                horizontalAlignment: Image.AlignHCenter
                verticalAlignment: Image.AlignTop
                source: "file:///C:/Users/admin/OneDrive - University of Florida/Documents/_Tuning/Gauge Cluster/needle.svg"
                rotation: 0
                fillMode: Image.PreserveAspectCrop
                sourceSize.width: 100
                transformOrigin: Item.Bottom
                sourceSize.height: 300
            }
        }

        Rectangle {
            id: fuelNeedle
            objectName: "fuelneedle"
            x: 170
            y: 200
            width: 100
            height: 500
            visible: true
            color: "#005d2d2d"
            scale: 1
            rotation: -90
            Image {
                id: fuelNeedlePic
                x: 0
                y: 120
                width: 100
                height: 130
                horizontalAlignment: Image.AlignHCenter
                verticalAlignment: Image.AlignTop
                source: "file:///C:/Users/admin/OneDrive - University of Florida/Documents/_Tuning/Gauge Cluster/needle.svg"
                rotation: 0
                sourceSize.width: 100
                fillMode: Image.PreserveAspectCrop
                transformOrigin: Item.Bottom
                sourceSize.height: 300
            }

            Rectangle {
                id: fuelAlignment
                width: 100
                height: 1
                color: "#ffffff"
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                visible: false
            }
        }

    }
    Rectangle {
            id: rectangle1
            x: 626
            y: 86
            width: 668
            height: 550
            color: "#0021202c"

            Rectangle {
                id: textbox
                y: 298
                width: 500
                height: 250
                color: "#181719"
                border.color: "#b4811e1e"
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
                    readOnly: true
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
                color: "#181719"
                border.color: "#b4811e1e"
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
                    readOnly:true
                    anchors.verticalCenter: parent.verticalCenter
                    enabled: true
                    anchors.horizontalCenter: parent.horizontalCenter
                    placeholderText: qsTr("")
                }
            }
        }

        Button {
            id: defButton
            x: 706
            y: 32
            width: 118
            height: 48
            background: Rectangle{
            radius: 10
            }
            text: qsTr("Defs")
            onClicked:{
                var component = Qt.createComponent("DefinitionWindow.qml")
                var win = component.createObject(root)
                win.show

            }
        }

        Image {
            id: rightSignal
            x: 1320
            y: 125
            width: 64
            height: 64
            source: "file:///C:/Users/admin/OneDrive - University of Florida/Documents/_Tuning/Gauge Cluster/offSignal.svg"
            antialiasing: true
            mipmap: true
            sourceSize.height: 64
            sourceSize.width: 64
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: leftSignal
            x: 556
            y: 125
            width: 64
            height: 64
            source: "file:///C:/Users/admin/OneDrive - University of Florida/Documents/_Tuning/Gauge Cluster/offSignal.svg"
            mirror: true
            fillMode: Image.PreserveAspectFit
            antialiasing: true
            sourceSize.height: 64
            mipmap: true
            sourceSize.width: 64
        }

        Image {
            id: lightIndicator
            x: 1260
            y: 76
            width: 64
            height: 64
            source: "file:///C:/Users/admin/OneDrive - University of Florida/Documents/_Tuning/Gauge Cluster/lightsOn.svg"
            scale: 0.7
            antialiasing: true
            mipmap: true
            sourceSize.height: 64
            sourceSize.width: 64
            fillMode: Image.PreserveAspectFit
        }
}

Button {
    id: logBtn
    x: 1094
    y: 34
    width: 118
    height: 48
    text: qsTr("Start Logging")
    background: Rectangle {
        radius: 10
    }
    onClicked: {
        logger.startLogging()
        console.log("logging")
    }
}
}
/*##^##
Designer {
    D{i:0;formeditorZoom:0.75}D{i:9;invisible:true}D{i:19}
}
##^##*/


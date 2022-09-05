

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
        id: rectangle
        width: 1920
        height: 720
        color: "#131313"

        Image {
            id: tach
            x: 0
            y: 0
            width: 720
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
                width: 100
                height: 600
                visible: true
                color: "#005d2d2d"
                anchors.verticalCenter: parent.verticalCenter
                scale: 1.1
                anchors.horizontalCenter: parent.horizontalCenter
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

            Label {
                id: rpmText
                objectName: "rpmText"
                y: 420
                width: 172
                height: 72
                color: "#c8c8c8"
                text: qsTr("0")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.family: "NI7SEG"
                font.bold: true
                font.pointSize: 44
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        Image {
            id: speedo
            x: 1200
            y: -8
            width: 720
            height: 720
            visible: true
            anchors.right: parent.right
            source: "file:///C:/Users/admin/OneDrive - University of Florida/Documents/_Tuning/Gauge Cluster/speedo.png"
            anchors.rightMargin: 0
            sourceSize.height: 720
            sourceSize.width: 620
            antialiasing: true
            mipmap: true
            fillMode: Image.PreserveAspectCrop

            Rectangle {
                id: speedoNeedle
                objectName: "speedoneedle"
                width: 100
                height: 600
                visible: true
                color: "#005d2d2d"
                anchors.verticalCenter: parent.verticalCenter
                scale: 1.1
                anchors.horizontalCenter: parent.horizontalCenter
                rotation: 270
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
                x: 210
                y: 207
                width: 100
                height: 500
                visible: true
                color: "#005d2d2d"
                scale: 1
                rotation: -90

                Rectangle {
                    id: fuelAlignment
                    x: 143
                    y: 340
                    width: 100
                    height: 1
                    color: "#ffffff"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    visible: false
                }

                Image {
                    id: fuelNeedlePic
                    x: 0
                    y: 120
                    width: 100
                    height: 130
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignTop
                    source: "file:///C:/Users/admin/OneDrive - University of Florida/Documents/_Tuning/Gauge Cluster/needle.svg"
                    scale: 1.2
                    rotation: 0
                    sourceSize.width: 100
                    fillMode: Image.PreserveAspectCrop
                    transformOrigin: Item.Bottom
                    sourceSize.height: 300
                }
            }

        }
        Rectangle {
            id: rectangle1
            width: 500
            height: 550
            color: "#0021202c"
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter

            Rectangle {
                id: centerBoxMain
                x: 84
                y: -86
                width: 500
                height: parent.height
                radius:10
                color: "#131313"
                border.color: "#3b3b3b"
                border.width: 2
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.horizontalCenter: parent.horizontalCenter

                Image {
                    id: bottomRedBox
                    width: 490
                    height: 170
                    y: 375
                    clip:true
                    source: "file:///C:/Users/admin/OneDrive - University of Florida/Documents/_Tuning/Gauge Cluster/centerRedBox.png"
                    anchors.horizontalCenter: parent.horizontalCenter

                    sourceSize.width: 550
                    fillMode: Image.Stretch
                }

                Image {
                    id: topRedBox
                    y: 5
                    width: 490
                    height: 170
                    clip:true
                    source: "file:///C:/Users/admin/OneDrive - University of Florida/Documents/_Tuning/Gauge Cluster/centerRedBox.png"
                    rotation: 180
                    anchors.horizontalCenter: parent.horizontalCenter
                    sourceSize.width: 550
                    fillMode: Image.Stretch

                    Rectangle {
                        id: speedoHandler
                        width: 480
                        rotation: 180
                        height: 150
                        color: "#00ffffff"
                        border.color: "#00000000"
                        border.width: 0
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter

                        Label {
                            id: speedText
                            x: 182
                            y: 39
                            objectName: "speedText"
                            width: speedText.paintedWidth
                            height: 72
                            color: "#c8c8c8"
                            text: qsTr("0")
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter

                            font.bold: true
                            font.pointSize: 72
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.family: "NI7SEG"
                        }

                        Label {
                            id: mphText
                            x: 162
                            y: 39
                            width: 80
                            height: 30
                            color: "#c8c8c8"
                            text: qsTr("MPH")
                            anchors.bottom: speedText.bottom
                            anchors.left: speedText.right
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter


                            objectName: "speedText"
                            font.pointSize: 26
                            font.family: "NI7SEG"
                            font.bold: true
                        }
                    }
                }

                Text {
                    id: statusText
                    width: 453
                    height: 140
                    visible: false
                    color: "#c8c8c8"
                    text: qsTr("Error Message")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 55
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    lineHeightMode: Text.ProportionalHeight
                    wrapMode: Text.WordWrap
                    fontSizeMode: Text.VerticalFit
                    font.family: "NI7SEG"
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }

        Button {
            id: defButton
            x: 710
            y: 44
            width: 118
            height: 40
            background: Rectangle{
                radius: 10
                color: "#5c5c5c"
            }
            text: qsTr("Defs")
            icon.color: "#000000"
            flat: false
            highlighted: false
            onClicked:{
                var component = Qt.createComponent("DefinitionWindow.qml")
                var win = component.createObject(root)
                win.show

            }


        }

        Button {
            id: logBtn
            x: 1090
            y: 44
            width: 118
            height: 40
            text: qsTr("Start Logging")
            background: Rectangle {
                radius: 10
                color: "#5c5c5c"
            }
            onClicked: {
                logger.startLogging()
            }
        }

        Image {
            id: rightSignal
            x: 1216
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
            x: 640
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
}
/*##^##
Designer {
    D{i:0;formeditorZoom:0.9}D{i:3}D{i:7}D{i:17}D{i:18}D{i:15}D{i:12}
}
##^##*/


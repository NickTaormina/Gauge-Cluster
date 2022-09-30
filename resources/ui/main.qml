

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
            source: "file:///" + applicationDirPath + "/resources/images/tach.png"
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
                    source: "file:///" + applicationDirPath + "/resources/images/needle.svg"
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
                width: 189
                height: 72
                color: "#c8c8c8"
                text: qsTr("0")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.family: "Michroma"
                font.bold: true
                font.pointSize: 36
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
            source: "file:///" + applicationDirPath + "/resources/images/speedo.png"
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
                    source: "file:///" + applicationDirPath + "/resources/images/needle.svg"
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
                    source: "file:///" + applicationDirPath + "/resources/images/needle.svg"
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
                    source: "file:///" + applicationDirPath + "/resources/images/centerRedBox.png"
                    anchors.horizontalCenter: parent.horizontalCenter

                    sourceSize.width: 550
                    fillMode: Image.Stretch

                    Rectangle {
                        id: gearIndicatorBox
                        x: 213
                        width: 96
                        height: 72
                        color: "#00ffffff"
                        radius: 10
                        border.color: "#c8c8c8"
                        border.width: 3
                        anchors.top: parent.top
                        anchors.topMargin: 5
                        anchors.horizontalCenter: parent.horizontalCenter

                        Text {
                            id: gearText
                            objectName: "gearText"
                            width: 81
                            height: 58
                            color: "#c8c8c8"
                            text: qsTr("N")
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: 70
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            anchors.verticalCenterOffset: -3
                            fontSizeMode: Text.FixedSize
                            font.family: "Exo ExtraBold"
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                    }

                    Rectangle {
                        id: bottomBoxSep
                        width: 480
                        height: 1
                        color: "#ffffff"
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.verticalCenterOffset: -5
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Rectangle {
                        id: warningBox
                        width: 56
                        height: 56
                        visible: false
                        color: "#00ffffff"
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        anchors.leftMargin: 12
                        anchors.bottomMargin: 16

                        Image {
                            id: warningImage
                            width: 56
                            height: 56
                            source: "qrc:/qtquickplugin/images/template_image.png"
                            sourceSize.width: 56
                            fillMode: Image.PreserveAspectFit
                        }
                    }

                    Text {
                        id: odoNum
                        objectName: "odoNum"
                        x: 143
                        y: 134
                        width: 166
                        height: 36
                        color: "#c8c8c8"
                        text: qsTr("077500")
                        font.pixelSize: 28
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.family: "Michroma"
                        fontSizeMode: Text.FixedSize
                    }

                    Text {
                        id: odoUnitText
                        x: 190
                        y: 135
                        width: 80
                        height: 24
                        color: "#c8c8c8"
                        text: qsTr("miles")
                        anchors.bottom: parent.bottom
                        font.pixelSize: 24
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        anchors.bottomMargin: 0
                        anchors.left: odoNum.right
                        fontSizeMode: Text.HorizontalFit
                        font.family: "Exo ExtraBold"
                    }

                    Text {
                        id: tripText
                        objectName: "tripNum"
                        y: 98
                        width: 198
                        height: 36
                        color: "#c8c8c8"
                        text: qsTr("0.0")
                        anchors.bottom: parent.bottom
                        font.pixelSize: 28
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                        anchors.horizontalCenterOffset: -2
                        anchors.bottomMargin: 36
                        anchors.horizontalCenter: parent.horizontalCenter
                        fontSizeMode: Text.FixedSize
                        font.family: "Michroma"

                        Image {
                            id: tripImage
                            width: 28
                            height: 28
                            visible: true
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            source: "file:///" + applicationDirPath + "/resources/images/tripImage.png"
                            anchors.leftMargin: 0
                            anchors.verticalCenterOffset: 2
                            sourceSize.height: 26
                            sourceSize.width: 26
                            fillMode: Image.PreserveAspectCrop
                            MouseArea{
                                anchors.fill: parent
                                onClicked: {
                                    gauge.resetTrip()

                                }
                            }
                        }
                    }

                }

                Image {
                    id: topRedBox
                    y: 5
                    width: 490
                    height: 170
                    clip:true
                    source: "file:///" + applicationDirPath + "/resources/images/centerRedBox.png"
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
                            height: 78
                            color: "#c8c8c8"
                            text: qsTr("0")
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter

                            font.bold: true
                            font.pointSize: 64
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.family: "Michroma"
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
                            font.family: "Exo ExtraBold"
                            font.bold: true
                        }
                    }
                }

                Text {
                    id: statusText
                    objectName: "statusText"
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
            objectName:"rightSignal"
            x: 1216
            y: 125
            width: 64
            height: 64
            source: "file:///" + applicationDirPath + "/resources/images/offSignal.svg"
            antialiasing: true
            mipmap: true
            sourceSize.height: 64
            sourceSize.width: 64
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: leftSignal
            objectName:"leftSignal"
            x: 640
            y: 125
            width: 64
            height: 64
            source: "file:///" + applicationDirPath + "/resources/images/offSignal.svg"
            mirror: true
            fillMode: Image.PreserveAspectFit
            antialiasing: true
            sourceSize.height: 64
            mipmap: true
            sourceSize.width: 64
        }

        Image {
            id: lightIndicator
            objectName:"lightIndicator"
            x: 1260
            y: 76
            width: 64
            height: 64
            source: "file:///" + applicationDirPath + "/resources/images/lightsOn.svg"
            scale: 0.7
            antialiasing: true
            mipmap: true
            sourceSize.height: 64
            sourceSize.width: 64
            fillMode: Image.PreserveAspectFit
        }

        Button {
            id: settingsBtn
            objectName: "settingsBtn"
            x: 901
            y: 44
            width: 118
            height: 40
            text: qsTr("Settings")
            background: Rectangle {
                color: "#5c5c5c"
                radius: 10
            }
            onClicked:{
                var component = Qt.createComponent("SettingsWindow.qml")
                var win = component.createObject(root)
                win.show

            }
        }



    }
}
/*##^##
Designer {
    D{i:0;formeditorZoom:0.9}D{i:15}D{i:21}D{i:22}
}
##^##*/


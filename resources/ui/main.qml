

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
    flags: Qt.FramelessWindowHint


    Rectangle {
        id: rectangle
        width: 1920
        height: 720
        color: "#111111"


        Rectangle {
            id: tachRect
            x: 485
            y: -319
            width: 950
            height: 720
            color: "#111111"
            border.color: "#00000000"
            border.width: 0
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter

            Image {
                id: image
                width: 1920
                height: 720
                anchors.verticalCenter: parent.verticalCenter
                source: "file:///" + applicationDirPath + "/resources/images/BMW-Z4-M40i-vs-Toyota-Supra-16-of-18.png"
                anchors.horizontalCenter: parent.horizontalCenter
                fillMode: Image.PreserveAspectCrop
            }
            Image {
                id: shiftLight
                objectName: "shiftLight"
                x: 247
                y: 119
                width: 407
                height: 496
                source: "file:///" + applicationDirPath + "/resources/images/shift light.png"
                visible: false
                rotation: 0
                sourceSize.height: 422
                sourceSize.width: 346
                fillMode: Image.PreserveAspectCrop
            }
            Rectangle {
                id: tachNeedleRect
                objectName: "tachneedle"
                width: 421
                height: 20
                color: "#00ffffff"
                border.color: "#00000000"
                anchors.verticalCenter: parent.verticalCenter
                rotation: -75
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.horizontalCenterOffset: 15

                Image {
                    id: needlePic
                    width: 35
                    height: 250
                    anchors.verticalCenter: parent.verticalCenter
                    source: "file:///" + applicationDirPath + "/resources/images/needle.png"
                    rotation: 270
                    anchors.verticalCenterOffset: 0
                    anchors.horizontalCenterOffset: -135
                    transformOrigin: Item.Center
                    sourceSize.height: 112
                    sourceSize.width: 75
                    anchors.horizontalCenter: parent.horizontalCenter
                    fillMode: Image.PreserveAspectCrop
                }
            }


        }

        Rectangle {
            id: gearIndicatorBox
            x: 202
            width: 150
            height: 150
            color: "#00ffffff"
            radius: 10
            border.color: "#00c8c8c8"
            border.width: 0
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter

            Label {
                id: gearIndicator
                objectName: "gearText"
                x: 31
                y: -19
                color: "#b53519"
                text: qsTr("2")
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenterOffset: -5
                anchors.verticalCenterOffset: 0
                font.pointSize: 100
                font.family: "Alaca Bold Italic"
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        Button {
            id: defButton
            x: 91
            y: 672
            width: 118
            height: 40
            background: Rectangle{
                radius: 10
                color: "#404040"
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
            x: 361
            y: 672
            width: 118
            height: 40
            text: qsTr("Start Logging")
            background: Rectangle {
                radius: 10
                color: "#404040"
            }
            onClicked: {
                logger.startLogging()
            }
        }

        Button {
            id: settingsBtn
            objectName: "settingsBtn"
            x: 228
            y: 672
            width: 118
            height: 40
            text: qsTr("Settings")
            background: Rectangle {
                color: "#404040"
                radius: 10
            }
            onClicked:{
                var component = Qt.createComponent("SettingsWindow.qml")
                var win = component.createObject(root)
                win.show

            }
        }

        Image {
            id: rightSignal
            objectName:"rightSignal"
            width: 64
            height: 64
            anchors.top: parent.top
            source: "file:///" + applicationDirPath + "/resources/images/offSignal.svg"
            anchors.topMargin: 15
            anchors.horizontalCenterOffset: 350
            anchors.horizontalCenter: parent.horizontalCenter
            antialiasing: true
            mipmap: true
            sourceSize.height: 64
            sourceSize.width: 64
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: leftSignal
            objectName:"leftSignal"
            width: 64
            height: 64
            anchors.top: parent.top
            source: "file:///" + applicationDirPath + "/resources/images/offSignal.svg"
            anchors.topMargin: 15
            anchors.horizontalCenterOffset: -350
            anchors.horizontalCenter: parent.horizontalCenter
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
            y: 76
            width: 64
            height: 64
            anchors.bottom: parent.bottom
            source: "file:///" + applicationDirPath + "/resources/images/lightsOff.svg"
            anchors.bottomMargin: -5
            anchors.horizontalCenterOffset: -300
            anchors.horizontalCenter: parent.horizontalCenter
            scale: 0.7
            antialiasing: true
            mipmap: true
            sourceSize.height: 64
            sourceSize.width: 64
            fillMode: Image.PreserveAspectFit
        }



        Text {
            id: odoNum
            objectName: "odoNum"
            x: 1308
            y: 574
            width: 166
            height: 36
            color: "#c8c8c8"
            text: qsTr("000000")
            font.pixelSize: 28
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.family: "Michroma"
            fontSizeMode: Text.FixedSize
        }

        Text {
            id: odoUnitText
            x: 314
            y: 586
            width: 80
            height: 24
            color: "#c8c8c8"
            text: qsTr("miles")
            anchors.bottom: parent.bottom
            font.pixelSize: 24
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.leftMargin: 0
            anchors.bottomMargin: 110
            anchors.left: odoNum.right
            fontSizeMode: Text.HorizontalFit
            font.family: "Exo ExtraBold"
        }

        Text {
            id: tripText
            x: 1600
            objectName: "tripNum"
            y: 540
            width: 198
            height: 36
            color: "#c8c8c8"
            text: qsTr("0.0")
            anchors.bottom: parent.bottom
            font.pixelSize: 28
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            anchors.horizontalCenterOffset: 415
            anchors.bottomMargin: 140
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
                        gauge.switchActiveTrip();

                    }
                }
            }
        }





        Rectangle {
            id: speedoHandler
            x: 720
            y: 285
            width: 480
            rotation: 0
            height: 150
            color: "#00ffffff"
            border.color: "#00000000"
            border.width: 0
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 0
            anchors.horizontalCenterOffset: -540
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

                anchors.right: mphText.left
                anchors.rightMargin: 2
                verticalAlignment: Text.AlignVCenter

                font.bold: true
                font.pointSize: 60

                font.family: "Michroma"
            }

            Label {
                id: mphText
                x: 350
                y: 39
                width: 80
                height: 30
                color: "#c8c8c8"
                text: qsTr("MPH")
                anchors.bottom: speedText.bottom

                verticalAlignment: Text.AlignVCenter


                objectName: "speedText"
                font.pointSize: 20
                font.family: "Exo ExtraBold"
                font.bold: true
            }
        }
        Rectangle {
            id: statusRect
            x: 1100
            y: 270
            width: 500
            height: 180
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 0
            color: "#00ffffff"
            border.color: "#00000000"
            border.width: 0

            Rectangle {
                id: verticalBar
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                x: 203
                y: 10
                width: 2
                height: 170
                color: "#6fffffff"
            }

            Rectangle {
                id: horizontalBar1
                x: 80
                y: 13
                width: 400
                height: 1
                color: "#6fffffff"
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter

            }

            Label {
                id: topLeftValue
                x: 45
                y: 40
                width: 200
                objectName: "topLeftValue"
                color: "#c5c5c5"
                text: qsTr("")
                font.family: "Michroma"
                font.pointSize: 24
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Label {
                id: topRightValue
                x: 255
                y: 40
                width: 200
                objectName: "topRightValue"
                color: "#c5c5c5"
                text: qsTr("")
                font.pointSize: 24
                font.family: "Michroma"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Label {
                id: bottomRightValue
                x: 255
                y: 120
                width: 200
                color: "#c5c5c5"
                objectName: "bottomRightValue"
                text: qsTr("")
                font.pointSize: 24
                font.family: "Michroma"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Label {
                id: bottomLeftValue
                x: 45
                y: 120
                width: 200
                color: "#c5c5c5"
                objectName: "bottomLeftValue"
                text: qsTr("")
                font.pointSize: 24
                font.family: "Michroma"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Label {
                id: topLeftLabel
                objectName: "topLeftLabel"
                x: 60
                y: 10
                width: 170
                height: 30
                color: "#c5c5c5"
                text: qsTr("AF Ratio")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.NoWrap
                font.pointSize: 14
                font.family: "Michroma"
            }
            Label {
                id: topRightLabel
                x: 275
                y: 10
                width: 170
                height: 30
                objectName: "topRightLabel"
                color: "#c5c5c5"
                text: qsTr("AF Learn")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.NoWrap
                font.pointSize: 14
                font.family: "Michroma"
            }
            Label {
                id: bottomRightLabel
                x: 275
                y: 90
                width: 170
                objectName: "bottomRightLabel"
                height: 30
                color: "#c5c5c5"
                text: qsTr("Oil Temp")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.NoWrap
                font.pointSize: 14
                font.family: "Michroma"
            }
            Label {
                id: bottomLeftLabel
                x: 60
                y: 90
                width: 170
                height: 30
                color: "#c5c5c5"
                objectName: "bottomLeftLabel"
                text: qsTr("Intake Temp")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.NoWrap
                font.pointSize: 14
                font.family: "Michroma"
            }
        }

        Image {
            id: coolantBarImage
            objectName: "coolantBarImage"
            x: 1595
            y: 425
            width: 220
            height: 210
            source: "file:///" + applicationDirPath + "/resources/images/coolantBar0.png"
            sourceSize.height: 179
            sourceSize.width: 182
            fillMode: Image.Stretch
        }
        Image {
            id: fuelBarImage
            objectName: "fuelBarImage"
            x: 130
            y: 420
            width: 210
            height: 200
            rotation: 0
            source: "file:///" + applicationDirPath + "/resources/images/fuelBar0.png"
            sourceSize.height: 168
            sourceSize.width: 175
            fillMode: Image.Stretch
        }






    }

    Label {
        id: clockText
        objectName: "clockText"
        x: 1200
        y: 138
        width: 200
        height: 50
        color: "#c5c5c5"
        text: qsTr("")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.family: "Michroma"
        font.pointSize: 22
    }

    Label {
        id: tempText
        objectName: "temperatureText"
        x: 1400
        y: 138
        width: 200
        height: 50
        color: "#c5c5c5"
        text: qsTr(" F")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 20
        font.family: "Michroma"
    }


}
/*##^##
Designer {
    D{i:0;formeditorZoom:0.9}D{i:8;invisible:true}D{i:7;invisible:true}D{i:10;invisible:true}
D{i:9;invisible:true}
}
##^##*/


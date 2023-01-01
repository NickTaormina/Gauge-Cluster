

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15




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
        Image {
            id: image
            width: 1920
            height: 720
            anchors.verticalCenter: parent.verticalCenter
            source: "file:///" + applicationDirPath + "/resources/images/bgWithLines.png"
            anchors.horizontalCenter: parent.horizontalCenter
            fillMode: Image.PreserveAspectCrop
        }
        Tach{
            id:tach
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
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
            id: lightIndicator
            objectName:"lightIndicator"
            property bool showBlue: false
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
            ColorOverlay{
                visible: lightIndicator.showBlue
                anchors.fill: lightIndicator
                source: lightIndicator
                color: "#284CED"
            }
        }
        Text {
            id: neutralText
            objectName:"neutralText"
            visible:false
            anchors.bottom:parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: 200
            font.pixelSize: 20
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            color: "#c8c8c8"
            font.family: "Michroma"
            text: qsTr("Neutral")
        }

        Text {
            id: clutchText
            objectName:"clutchText"
            visible: false
            anchors.bottom:parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: 500
            font.pixelSize: 20
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            color: "#c8c8c8"
            font.family: "Michroma"
            text: qsTr("clutch")
        }

        Text {
            id: refText
            objectName:"refText"

            anchors.bottom:parent.bottom
            anchors.horizontalCenter: parent.right
            anchors.horizontalCenterOffset: -50
            font.pixelSize: 20
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            color: "#c8c8c8"
            font.family: "Michroma"
            text: qsTr("msg")
        }


        Text {
            id: odoNum
            objectName: "odoNum"
            opacity:0
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
            opacity:odoNum.opacity
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
            opacity: odoNum.opacity
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
                opacity:odoNum.opacity
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




        CircularProgressBar{
            id: boostGauge
            objectName: "boostGauge"
            visible: false
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 175
            anchors.horizontalCenterOffset: -520
            anchors.horizontalCenter: parent.horizontalCenter
            progressWidth: 4
            strokeBgWidth: 2
            width: 75
            height: 75
            roundCap: false
            enableDropShadow: true
        }
        Rectangle {
            id: speedoHandler
            objectName: "speedoHandler"
            x: 720
            y: 285
            width: 480
            rotation: 0
            height: 150
            color: "#00ffffff"
            border.color: "#00000000"
            //opacity: 0
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
                opacity: 0
                height: 78
                color: "#c8c8c8"
                text: qsTr("0")
                anchors.verticalCenter: parent.verticalCenter

                anchors.right: mphText.left
                anchors.rightMargin: 2
                verticalAlignment: Text.AlignVCenter

                font.bold: true
                font.pointSize: 58

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
                opacity:speedText.opacity

                verticalAlignment: Text.AlignVCenter


                objectName: "speedText"
                font.pointSize: 20
                font.family: "Exo ExtraBold"
                font.bold: true
            }
        }
        Image {
            id: celImage
            objectName: "celImage"
            visible: false
            anchors.horizontalCenter: tach.horizontalCenter
            anchors.horizontalCenterOffset: 100
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 25
            source: "file:///" + applicationDirPath + "/resources/images/cel.png"
            width:  50
            sourceSize.height: 256
            sourceSize.width: 256
            fillMode: Image.PreserveAspectFit
            ColorOverlay {
                anchors.fill: celImage
                source: celImage
                color: "#ffcc00"
            }

        }
        Image {
            id: brakeImage
            objectName: "brakeImage"
            visible: false
            anchors.horizontalCenter: tach.horizontalCenter
            anchors.horizontalCenterOffset: 100
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 75
            source: "file:///" + applicationDirPath + "/resources/images/brake.png"
            width:  50
            sourceSize.height: 256
            sourceSize.width: 256
            fillMode: Image.PreserveAspectFit
        }
        Rectangle{
            id: cruiseRect
            objectName: "cruiseRect"
            width: 480
            rotation: 0
            height: 75
            color: "#00ffffff"
            border.color: "#00000000"
            //opacity: 0
            border.width: 0
            anchors.bottom: speedoHandler.top
            anchors.bottomMargin: 0
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: speedoHandler.horizontalCenter
            Label {
                id: cruiseText
                objectName: "cruiseText"
                width: cruiseText.paintedWidth
                visible: false
                height: 50
                color: "#468250"
                text: qsTr("49")
                anchors.verticalCenter: parent.bottom
                anchors.verticalCenterOffset: 15
                anchors.right: parent.right
                anchors.rightMargin: 140
                verticalAlignment: Text.AlignVCenter
                font.bold: true
                font.pointSize: 26
                font.family: "Michroma"
            }
            Image {
                id: cruiseImage
                objectName: "cruiseImage"
                height: 30
                visible: false
                anchors.verticalCenter: cruiseText.verticalCenter
                anchors.verticalCenterOffset: -2
                anchors.right: cruiseText.left
                source: "file:///" + applicationDirPath + "/resources/images/cruiseImage.png"
                anchors.rightMargin: 5
                sourceSize.height: 192
                sourceSize.width: 256
                fillMode: Image.PreserveAspectFit
            }
            Image {
                id: setImage
                objectName: "setImage"
                height: 8
                visible: false
                anchors.top: cruiseImage.bottom
                anchors.horizontalCenter: cruiseImage.horizontalCenter
                source: "file:///" + applicationDirPath + "/resources/images/cruiseSetImage.png"
                sourceSize.height: 50
                sourceSize.width: 256
                fillMode: Image.PreserveAspectFit
            }

        }

        Rectangle {
            id: statusRect
            objectName: "statusRect"
            x: 1120
            opacity: 1
            y: 270
            width: 500
            height: 280
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 0
            color: "#00ffffff"
            border.color: "#ffffff"
            border.width: 0


            Image {
                id: screenChangeUp
                visible: false
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.horizontalCenterOffset: .5
                anchors.verticalCenter: parent.top
                anchors.verticalCenterOffset: 5
                width: 25
                height: 35
                source: "file:///" + applicationDirPath + "/resources/images/arrowChevron.png"
                sourceSize.height: 512
                sourceSize.width: 512
                rotation: -90
                opacity: .3
                fillMode: Image.Stretch
                antialiasing: true
                smooth: true
            }
            ColorOverlay {
                anchors.fill: screenChangeUp
                rotation: -90
                opacity: .3
                source: screenChangeUp
                color: "#c8c8c8"
                visible: false
            }

            Image {
                id: screenChangeDown
                visible: false
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.horizontalCenterOffset: .5
                anchors.verticalCenter: parent.bottom
                anchors.verticalCenterOffset: 0
                width: 25
                height: 35
                source: "file:///" + applicationDirPath + "/resources/images/arrowChevron.png"
                sourceSize.width: 512
                rotation: 90
                opacity: .3
                fillMode: Image.Stretch
                sourceSize.height: 512
                smooth: true
                antialiasing: true

            }
            ColorOverlay {
                anchors.fill: screenChangeDown
                rotation: 90
                opacity: .3
                source: screenChangeDown
                color: "#c8c8c8"
                visible: false
            }



        }
        Image {
            id: coolantBarImage
            objectName: "coolantBarImage"
            x: 1595
            opacity:0
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
            opacity:coolantBarImage.opacity
            width: 210
            height: 200
            rotation: 0
            source: "file:///" + applicationDirPath + "/resources/images/fuelBar0.png"
            sourceSize.height: 168
            sourceSize.width: 175
            fillMode: Image.Stretch
        }

        Label {
            id: statusText
            objectName: "statusText"
            x: 1242
            y: 220
            width: 217
            height: 35
            color: "#c8c8c8"
            text: qsTr("Status Text")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 14
            font.family: "Michroma"
        }

        Slider {
            id: throttleSlider
            objectName: "throttleBar"
            x: 410
            opacity:0
            y: 580
            width: 250
            height: 20
            rotation: 0
            wheelEnabled: false
            focusPolicy: Qt.NoFocus
            hoverEnabled: false
            enabled: false
            orientation: Qt.Horizontal
            value: 0
            to: 1
            from: 0

            background: Rectangle{
                anchors.verticalCenter: parent.verticalCenter

                y: parent.topPadding + parent.availableHeight/2 - height/2
                implicitWidth: parent.width
                implicitHeight: parent.height
                width: parent.availableWidth
                height: implicitHeight
                color: "#00111111"
                border.width: 2
                border.color: "#333333"
                radius: 2
            }

            Rectangle {
                y: 1
                height: parent.height - 2*1
                width: parent.visualPosition*parent.width
                color: "#445544"
                radius: 2
            }
            handle: Rectangle {
                implicitWidth: 0
                implicitHeight: 0
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
            opacity: 0
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
 }
/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.9;height:480;width:640}D{i:10;invisible:true}
D{i:12;invisible:true}D{i:11;invisible:true}D{i:13;invisible:true}
}
##^##*/


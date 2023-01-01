import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15


Item {
    id: baseTachItem

        Image {
            id: tachImage
            height:827
            width:650
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 15
            source: "file:///" + applicationDirPath + "/resources/images/tach.png"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: -47
            fillMode: Image.PreserveAspectCrop
        }


        Rectangle {
            id: gearIndicatorBox
            x: 215
            width: 150
            height: 150
            color: "#00ffffff"
            radius: 10
            border.color: "#00c8c8c8"
            border.width: 0
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: 10

            Image {
                id: rightSignal
                objectName:"rightSignal"
                width: 64
                height: 64
                visible: false
                anchors.top: parent.top
                source: "file:///" + applicationDirPath + "/resources/images/onSignal.svg"
                opacity: 1
                anchors.topMargin: -40
                anchors.horizontalCenterOffset: 50
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
                visible: false
                anchors.top: parent.top
                source: "file:///" + applicationDirPath + "/resources/images/onSignal.svg"
                anchors.topMargin: -40
                opacity: 1
                anchors.horizontalCenterOffset: -50
                anchors.horizontalCenter: parent.horizontalCenter
                mirror: true
                fillMode: Image.PreserveAspectFit
                antialiasing: true
                sourceSize.height: 64
                mipmap: true
                sourceSize.width: 64
            }
            Label {
                id: gearIndicator
                objectName: "gearText"
                opacity:0
                x: 31
                y: -19
                color: "#b53519"
                text: qsTr("N")
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenterOffset: -5
                anchors.verticalCenterOffset: 0
                font.pointSize: 100
                font.family: "Alaca Bold Italic"
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
        Image {
            id: shiftLight
            objectName: "shiftLight"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenterOffset: -22.5
            anchors.verticalCenterOffset: 8
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
            anchors.verticalCenterOffset: 5
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

        Rectangle {
            id: shiftTargetRect
            width: 450
            height: 20
            color: "#00ffffff"
            border.color: "#00000000"
            anchors.verticalCenter: parent.verticalCenter
            objectName: "shiftTargetRect"
            visible: false
            rotation: 12.65
            anchors.horizontalCenterOffset: 15
            Image {
                id: shiftTargetPic
                width: 13
                height: 60
                anchors.verticalCenter: parent.verticalCenter
                source: "file:///" + applicationDirPath + "/resources/images/shift target.png"
                anchors.verticalCenterOffset: 0
                sourceSize.height: 72
                anchors.horizontalCenterOffset: -225
                rotation: 269.5
                sourceSize.width: 20
                transformOrigin: Item.Center
                anchors.horizontalCenter: parent.horizontalCenter
                fillMode: Image.PreserveAspectFit
            }
            anchors.horizontalCenter: parent.horizontalCenter
        }


    }


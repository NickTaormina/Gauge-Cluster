import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: rootItem
    objectName:"rootItem"
    width: 210
    height: 200

    Rectangle {
        id: textRectTop
        objectName:"textRectTop"
        width: parent.width
        height: parent.height/2
        color: "#00ffffff"
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -parent.height/4
        anchors.horizontalCenter: parent.horizontalCenter

        Text {
            id: paramLabel1
            width: parent.width
            height:30
            objectName:"paramLabel1"
            text: qsTr("")
            color: "#c5c5c5"
            elide: Text.ElideMiddle
            anchors.top: parent.top
            font.pointSize: 14
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.NoWrap
            rightPadding: 10
            leftPadding: 10
            fontSizeMode: Text.FixedSize
            anchors.topMargin: 5
            font.family: "Michroma"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            id: paramText1
            width:parent.width
            objectName:"paramValue1"
            color: paramLabel1.color
            text: qsTr("")
            anchors.top:paramLabel1.bottom
            font.pointSize: 24
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.family: paramLabel1.font.family
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            id: topMin
            width: 100
            color: paramLabel1.color
            text: qsTr("")
            anchors.bottom: parent.bottom
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.bottomMargin: 5
            anchors.horizontalCenterOffset: -(parent.width/4)
            font.family: paramLabel1.font.family
            objectName: "topMin"
            anchors.horizontalCenter: parent.horizontalCenter
            font.pointSize: 10
        }

        Rectangle {
            id: topMinMaxSep
            x: -21
            y: 163
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom:topMin.bottom
            anchors.bottomMargin: 3
            visible:topMin.visible
            width: 1
            height: 8
            color: "#6fffffff"
        }

        Text {
            id: topMax
            width: 100
            color: paramLabel1.color
            text: qsTr("")
            anchors.bottom: topMin.bottom
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.horizontalCenterOffset: -topMin.anchors.horizontalCenterOffset
            font.family: paramLabel1.font.family
            objectName: "topMax"
            anchors.horizontalCenter: parent.horizontalCenter
            font.pointSize: 10
        }

    }

    Rectangle {
        id: textRectBottom
        objectName:"textRectBottom"
        width: parent.width
        height: parent.height/2
        color: "#00ffffff"
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: parent.height/4
        anchors.horizontalCenter: parent.horizontalCenter
        Text {
            id: paramLabel2
            width: parent.width
            height:30
            objectName:"paramLabel2"
            text: qsTr("")
            color: paramLabel1.color
            elide: Text.ElideMiddle
            anchors.top: parent.top
            font.pointSize: paramLabel1.font.pointSize
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.NoWrap
            rightPadding: 10
            leftPadding: 10
            fontSizeMode: Text.FixedSize
            anchors.topMargin: 1
            font.family: paramLabel1.font.family
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            id: paramText2
            width:parent.width
            objectName:"paramValue2"
            color: paramText1.color
            text: qsTr("")
            anchors.top:paramLabel2.bottom
            font.pointSize: paramText1.font.pointSize
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: paramText1.font.family
        }

        Text {
            id: bottomMin
            width: 100
            color: paramLabel1.color
            text: qsTr("")
            anchors.bottom: parent.bottom
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.bottomMargin: 10
            anchors.horizontalCenterOffset: -(parent.width/4)
            font.family: paramLabel1.font.family
            objectName: "bottomMin"
            anchors.horizontalCenter: parent.horizontalCenter
            font.pointSize: 10
        }

        Rectangle {
            id: bottomMinMaxSep
            x: -21
            y: 128
            visible: bottomMin.visible
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom:bottomMin.bottom
            anchors.bottomMargin: topMinMaxSep.anchors.bottomMargin
            width: 1
            height: topMinMaxSep.height
            color: topMinMaxSep.color
        }

        Text {
            id: bottomMax
            width: 100
            color: paramLabel1.color
            text: qsTr("")
            anchors.bottom: bottomMin.bottom
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.horizontalCenterOffset: -topMin.anchors.horizontalCenterOffset
            font.family: paramLabel1.font.family
            objectName: "bottomMax"
            anchors.horizontalCenter: parent.horizontalCenter
            font.pointSize: 10
        }

    }

    Rectangle {
        id: horizRect
        objectName:"horizRect"
        width: parent.width
        height: 2
        color: "#6fffffff"
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Rectangle {
        id: verticalRect
        objectName:"verticalRect"
        x: 33
        y: 153
        width: 2
        height: parent.height - (parent.height*.15)
        anchors.right:parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 2
        color: "#6fffffff"
    }

}

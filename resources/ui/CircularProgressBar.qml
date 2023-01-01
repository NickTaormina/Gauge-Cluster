import QtQuick 2.15
import QtQuick.Shapes 1.15
import QtGraphicalEffects 1.15
//taken from: https://github.com/Wanderson-Magalhaes/Circular_ProgressBar

Item {
    id: progress
    implicitWidth: 250
    implicitHeight: 250

    // Properties
    // General
    property bool roundCap: true
    property int startAngle: -225
    property int maxAngle: 270
    property real maxValue: 100
    property real value: 0
    property int samples: 20
    // Drop Shadow
    property bool enableDropShadow: false
    property color dropShadowColor: "#20000000"
    property int dropShadowRadius: 10
    // Bg Circle
    property color bgColor: "transparent"
    property color bgStrokeColor: "#333333"
    property int strokeBgWidth: 16
    // Progress Circle
    property color progressColor: "#b53519"
    property int progressWidth: 16
    // Text
    property string text: ""
    property bool textShowValue: false
    property string textFontFamily: "Michroma"
    property int textSize: 11
    property color textColor: "#c8c8c8"

    // Internal Properties/Functions
    QtObject{
        id: internal

        property Component dropShadow: DropShadow{
            color: progress.dropShadowColor
            fast: true
            verticalOffset: 0
            horizontalOffset: 0
            samples: progress.samples
            radius: progress.dropShadowRadius
        }
    }


    Shape{
        id: shape
        anchors.fill: parent
        layer.enabled: true
        layer.samples: progress.samples
        layer.effect: progress.enableDropShadow ? internal.dropShadow : null

        ShapePath{
            id: pathBG
            strokeColor: progress.bgStrokeColor
            fillColor: progress.bgColor
            strokeWidth: progress.strokeBgWidth
            capStyle: progress.roundCap ? ShapePath.RoundCap : ShapePath.FlatCap

            PathAngleArc{
                radiusX: (progress.width / 2) - (progress.progressWidth / 2)
                radiusY: (progress.height / 2) - (progress.progressWidth / 2)
                centerX: progress.width / 2
                centerY: progress.height / 2
                startAngle: progress.startAngle
                sweepAngle: progress.maxAngle
            }
        }

        ShapePath{
            id: path
            strokeColor: progress.progressColor
            fillColor: "transparent"
            strokeWidth: progress.progressWidth
            capStyle: progress.roundCap ? ShapePath.RoundCap : ShapePath.FlatCap

            PathAngleArc{
                radiusX: (progress.width / 2) - (progress.progressWidth / 2)
                radiusY: (progress.height / 2) - (progress.progressWidth / 2)
                centerX: progress.width / 2
                centerY: progress.height / 2
                startAngle: progress.startAngle
                sweepAngle: (progress.maxAngle / progress.maxValue * progress.value)
            }
        }

        Text {
            id: textProgress
            text: progress.text
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: progress.textColor
            font.pointSize: progress.textSize
            font.family: progress.textFontFamily
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:250;width:250}
}
##^##*/

import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: musicPlayerItem
    width: 500
    height: 200

    ProgressBar {
        id: progressBar
        y: 130
        width: 450
        height: 5
        anchors.horizontalCenter: parent.horizontalCenter
        value: 0.6
    }

    Image {
        id: previousImage
        x: 75
        y: 150
        width: 40
        height: 40
        source: "back.png"
        mipmap: true
        antialiasing: true
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: playPauseImage
        y: 150
        width: 40
        height: 40
        source: "play.png"
        antialiasing: true
        mipmap: true
        anchors.horizontalCenter: parent.horizontalCenter
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: skipImage
        x: 385
        y: 150
        width: 40
        height: 40
        source: "skip.png"
        antialiasing: true
        mipmap: true
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: albumImage
        x: 25
        y: 8
        width: 100
        height: 100
        source: "qrc:/qtquickplugin/images/template_image.png"
        fillMode: Image.PreserveAspectFit
    }

    Text {
        id: titleText
        x: 130
        y: 14
        width: 365
        height: 50
        text: qsTr("Miracle (Above and Beyond Club Mix)")
        elide: Text.ElideRight
        font.pixelSize: 20
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        lineHeightMode: Text.ProportionalHeight
        wrapMode: Text.WordWrap
        fontSizeMode: Text.FixedSize
        font.family: "Michroma"
    }

    Text {
        id: artistText
        x: 130
        y: 70
        width: 365
        height: 40
        text: qsTr("Above & Beyond")
        elide: Text.ElideRight
        font.pixelSize: 16
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.family: "Michroma"
    }
}



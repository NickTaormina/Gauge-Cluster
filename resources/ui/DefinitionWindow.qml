import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3


Item {
    id: defMainItem
    width: 720
    height: 600
    opacity: 1
    anchors.centerIn: parent
    FileDialog{
        id: fileDialog
        title: "Select definition file"
        defaultSuffix: "xml"
        nameFilters: ["Definition Files (*.xml)"]
        onAccepted: {
                console.log("You chose: " + fileDialog.fileUrls)
                defClass.setDefPath(fileDialog.fileUrls)
                defClass.parseDefs()
                close()
            }
        onRejected: {
            console.log("Canceled")
            close()
        }
    }
    Rectangle {
        id: rectangle
        width: 720
        height: 600
        color: "#d7d7d7"
        radius: 10
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        Component{
            id: defItem
            Rectangle{
                Text{
                }
            }
        }

        ListView {
            id: listView
            width: 492
            height: 584
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 8
            model: defClass.params
            delegate: Item {
                id:itemdel
                x: 0
                width: 500
                height: 35
                Rectangle{
                    id:itemRect
                    width:parent.width
                    height:parent.height
                    radius: 5
                    Row {
                        id: row1
                        spacing: 10
                        Rectangle{
                            id: checkBox
                            x: 3
                            y: 3
                            width:30
                            height:30
                            border.color: "black"
                            border.width: 1
                            radius: 3
                            anchors.verticalCenter: parent.verticalCenter
                            Component.onCompleted: {
                                if(defClass.isSelected(modelData)){
                                    checkBox.color = "black"
                                    console.log("already selected: " + modelData)
                                }
                            }
                        }
                        Text {
                            text: modelData
                            anchors.verticalCenter: parent.verticalCenter
                            MouseArea{
                                anchors.fill: parent
                                onClicked: {
                                    listView.currentIndex = index
                                    if(defClass.isSelected(modelData)){
                                        checkBox.color = "white"
                                        console.log("unselected: " + modelData)
                                        defClass.removeSelectedParam(modelData)

                                    } else {
                                        checkBox.color = "black"
                                        console.log(modelData)
                                        defClass.setSelectedParams(modelData)
                                    }


                                }


                            }
                            font.bold: true
                            font.pixelSize: 20
                        }
                    }
                }
            }


        }

        Button {
            id: acceptBtn
            x: 552
            y: 33
            width: 131
            height: 29
            background: Rectangle{
            radius: 10}
            text: qsTr("Accept")
            onClicked: {
                defClass.printSelectedParams()
                defClass.fillDefs()
                defMainItem.destroy()
            }
        }

        Button {
            id: cancelBtn
            x: 552
            y: 120
            width: 131
            height: 31
            background: Rectangle{
                radius: 10}
            text: qsTr("Cancel")
            onClicked: {
                defClass.clearSelected()
                defMainItem.destroy()
            }
        }

        Button {
            id: openBtn
            x: 552
            y: 198
            width: 131
            height: 31
            text: qsTr("Open")
            background: Rectangle {
                radius: 10
            }

            onClicked: {

                fileDialog.open()
            }
        }

        Button {
            id: clearBtn
            x: 552
            y: 270
            width: 131
            height: 31
            text: qsTr("Clear")
            background: Rectangle {
                radius: 10
            }
            onClicked: {
                defClass.clearSelected()
                listView.update()
                defMainItem.update()

            }
        }

        Button {
            id: clearBtn1
            x: 552
            y: 336
            width: 131
            height: 31
            text: qsTr("test sweep")
            background: Rectangle {
                radius: 10
            }
            onClicked: {
                defClass.printSelectedParams()
                defClass.fillDefs()
                defClass.startTestSweep()
                defMainItem.destroy()
            }
        }

    }

}



/*##^##
Designer {
    D{i:0;formeditorZoom:1.1}D{i:21}
}
##^##*/

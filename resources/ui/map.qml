import QtQuick 2.15
import QtGraphicalEffects 1.15
import QtLocation 5.15
import QtPositioning 5.15

Item {
    id: rootItem
    objectName:"mapItem"
    width: parent.width
    height: parent.height
    Map {
        id: map
        visible:false
        anchors.fill: parent
        plugin: Plugin { name: "mapboxgl"
            PluginParameter{name: "mapboxgl.access_token"; value: "eyJ1Ijoibmlja3Rhb3JtaW5hIiwiYSI6ImNsYnd5aDlxNDBlMzQzcHFkMGJ6cjVrZ3EifQ.2YlXvQbYZAPnEBNB_seA3Q"}

        }
        center: QtPositioning.coordinate(29.6098, -82.371)
        zoomLevel: 12
    }
}

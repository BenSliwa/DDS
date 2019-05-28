import QtQuick 2.0
import QtCharts 2.0
import QtLocation 5.6
import QtPositioning 5.6

MapQuickItem {
    anchorPoint.x: image.width/4
    anchorPoint.y: image.height
    sourceItem: Image { id: image; source: "qrc:/ui/img/marker.png" }
    z: 5
}

import QtQuick 2.0
import QtCharts 2.0
import QtLocation 5.6
import QtPositioning 5.6

MapQuickItem {
    anchorPoint.x: image.width/2
    anchorPoint.y: image.height/2
    sourceItem: Image { id: image; source: "qrc:/ui/img/car.png" }
    z: 10
}

import QtQuick 2.0
import QtCharts 2.0
import QtLocation 5.6
import QtPositioning 5.6

Item
{
    property variant car
    property variant prediction

    function createObject(_path, _parent)
    {
        var component = Qt.createComponent(_path);
        if(component.status==Component.Ready)
            return component.createObject(_parent);
    }

    Component.onCompleted:
    {
        UiManager.positionUpdated.connect(setPosition);
        UiManager.predictionUpdated.connect(onPredictionUpdated);

        car = createObject("qrc:/ui/qml/Car.qml", map);
        map.addMapItem(car);

        prediction = createObject("qrc:/ui/qml/Marker.qml", map);
        map.addMapItem(prediction);
    }


    id: page
    width: 1600
    height: 800

    focus: true
    Keys.onPressed:
    {
        UiManager.handleKeyPress(event.key)
    }

    Plugin {
        id: mapPlugin
        name: "osm"
        PluginParameter { name: "osm.mapping.providersrepository.address"; value: "http://maps-redirect.qt.io/osm/5.8/street"}
    }

    Map {
        Image { id: image; width: 75; height: width; source: "qrc:/ui/img/logo.png" }

        id: map
        width: 1600 //page.width*2/3
        height: 800 //page.height-header.height
        plugin: mapPlugin
        zoomLevel: 16
    }

    function setPosition(_lat, _lon)
    {
        var pos = QtPositioning.coordinate(_lat, _lon);
        map.center = pos;
        car.coordinate = pos;
    }

    function onPredictionUpdated(_lat, _lon)
    {
        var pos = QtPositioning.coordinate(_lat, _lon);
        prediction.coordinate = pos;
    }
}

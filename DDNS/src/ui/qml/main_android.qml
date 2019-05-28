import QtQuick 2.0
import QtQuick.Controls 2.0
import QtCharts 2.0

Rectangle
{
    id: page
    width: 500
    height: 500
    color: "white"

    Component.onCompleted:
    {
        mnoField.text = NetworkContext.getCarrier();
        UiManager.transmitted.connect(onTransmitted);
        UiManager.started.connect(onStarted);
        UiManager.stopped.connect(onStopped);
    }

    function onTransmitted(_throughput, _dir)
    {
        if(_dir=="ul")
            ulField.text = _throughput.toFixed(2);
        else
            dlField.text = _throughput.toFixed(2);
    }

    function onStarted()
    {
        button.text = "Stop";
    }

    function onStopped()
    {
        button.text = "Start";
    }

    Column
    {
        id: plotColumn

        Image
        {
            id: logo
            anchors.horizontalCenter: parent.horizontalCenter
            source: "qrc:/ui/img/logo.png"
            width: page.width * 0.7
            fillMode: Image.PreserveAspectFit
        }

        Button
        {
            id: button
            width: page.width
            height: logo.height
            text: "Start"

            onClicked:
            {
                if(text=="Start")
                {
                    UiManager.start();
                }
                else
                {
                    UiManager.stop();
                }
            }
        }

        Text
        {
            id: mnoField
            text: "unknown MNO"
        }

        Row
        {
            Text {
                text: "UL "
            }
            Text
            {
                id: ulField
                text: ""
            }
        }

        Row
        {
            Text {
                text: "DL "
            }
            Text
            {
                id: dlField
                text: ""
            }
        }


    }


}

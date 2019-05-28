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
        UiManager.transmitted.connect(onTransmitted);
    }



    function onTransmitted(_throughput, _dir)
    {
        if(_dir=="UL")
            ulField.text = _throughput.toFixed(2);
        else
            dlField.text = _throughput.toFixed(2);
    }

    Column
    {
        id: plotColumn

        Image
        {
            id: logo
            anchors.horizontalCenter: parent.horizontalCenter
            source: "qrc:/img/sfb_logo.png"
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
                    text = "Stop";
                    UiManager.start();
                }
                else
                {
                    text = "Start";
                    UiManager.stop();
                }
            }
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

import QtQuick 2.0
import QtCharts 2.0

ChartView {
    width: page.width
    height: (page.height - logo.height - button.height) / (plotColumn.children.length-2)
    antialiasing: true
    legend.visible: false

    function init(xMin, xMax, xLabel, y0Min, y0Max, y0Label, y1Min, y1Max, y1Label)
    {
        xAxis.min = xMin;
        xAxis.max = xMax;
        xAxis.titleText = xLabel;

        y0Axis.min = y0Min;
        y0Axis.max = y0Max;
        y0Axis.titleText = y0Label;

        y1Axis.min = y1Min;
        y1Axis.max = y1Max;
        y1Axis.titleText = y1Label;

        left.clear();
        right.clear();
    }

    function add(_x, _y0, _y1)
    {
        left.add(_x, _y0);
        right.add(_x, _y1);
    }

    LineSeries {
        id: left
        width: 4
        color: "blue"

        function add(_x, _y)
        {
            append(_x, _y);
            if(count>2)
            {
                var filtered = false;
                while(at(count-1).x-at(0).x>50)
                {
                    remove(0);
                    filtered = true;
                }

                if(filtered)
                {
                    xAxis.min = at(0).x;
                    xAxis.max = _x;
                }
            }
        }

        axisX: ValueAxis
        {
            id: xAxis
        }
        axisY: ValueAxis
        {
            id: y0Axis
        }
    }

    LineSeries {
        id: right
        color: "red"
        width: 4

        function add(_x, _y)
        {
            append(_x, _y);
            if(count>2)
            {
                var filtered = false;
                while(at(count-1).x-at(0).x>50)
                    remove(0);
            }
        }

        axisYRight: ValueAxis
        {
            id: y1Axis
        }
    }
}

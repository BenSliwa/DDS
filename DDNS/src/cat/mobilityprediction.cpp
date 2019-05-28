#include "mobilityprediction.h"
#include "data/filehandler.h"
#include <math.h>

MobilityPrediction::MobilityPrediction(QObject *_parent)
{
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void MobilityPrediction::init(const QString &_scenario)
{
    m_trace.clear();

    QString file;
    #ifdef Q_OS_ANDROID
        if(_scenario=="suburban")
            file = "/sdcard/1547646477_vodafone_context.txt";
        else if(_scenario=="highway")
            file = "/sdcard/1544593310_vodafone_context.txt";
    #else
        if(_scenario=="suburban")
            file = "data/suburban/1547646477_vodafone_context.txt";
        else if(_scenario=="highway")
            file = "data/highway/1544593310_vodafone_context.txt";
    #endif

    QStringList lines = FileHandler::read(file);
    foreach(QString line, lines)
    {
        QVector<double> entry = FileHandler::parseCSV(line);
        if(entry.size()>5)
        {
            QVector2D position(entry.at(3), entry.at(4));
            m_trace << position;
        }
    }

    //qDebug() << "MobilityPrediction::init" << _scenario << m_trace.size();
}

QVector3D MobilityPrediction::findNearestPosition(double _lat, double _lon)
{
    QVector3D nearest;
    QVector2D current(_lat, _lon);

    double minDistance_m = 10000;
    for(int i=0; i<m_trace.size(); i++)
    {
        QVector2D position = m_trace.at(i);
        double distance_m = computeDistance(current, position);
        if(distance_m<minDistance_m)
        {
            minDistance_m = distance_m;
            nearest = QVector3D(position.x(), position.y(), i);
        }
    }

    return nearest;
}


QVector3D MobilityPrediction::predict(double _lat, double _lon, double _tau, double _velocity_mps)
{
    QVector3D nearest = findNearestPosition(_lat, _lon);
    QVector3D prediction = nearest;

    int index = nearest.z();
    double distancePotential_m = _tau * _velocity_mps;
    while(distancePotential_m>0)
    {
        if(index<m_trace.size()-1)
        {
            QVector2D current = m_trace.at(index);
            QVector2D next = m_trace.at(index + 1);
            double distance_m = computeDistance(current, next);

            if(distance_m<=distancePotential_m)
                distancePotential_m -= distance_m;
            else // interpolate
            {
                QVector3D dir = (next - current);
                prediction = current + dir * distancePotential_m / distance_m;
                prediction.setZ(index + distancePotential_m / distance_m);

                distancePotential_m = 0;
            }
        }
        else
            break;

        index++;
    }

    return prediction;
}

double MobilityPrediction::computeDistance(const QVector2D &_from, const QVector2D &_to)
{
    double distance_m = 0;

    double lat0 = toRad(_from.x());
    double lon0 = toRad(_from.y());
    double lat1 = toRad(_to.x());
    double lon1 = toRad(_to.y());

    double sinTerm = sin(lat0)*sin(lat1);
    double cosTerm = cos(lat0)*cos(lat1)*cos(lon1-lon0);

    distance_m = acos(sinTerm+cosTerm)*40000/360*1000;    // map to earth
    distance_m = toGrad(distance_m);

    return distance_m;
}

/*************************************
 *           PRIVATE METHODS         *
 ************************************/

double MobilityPrediction::toRad(double _grad)
{
    return _grad/180.0*3.141592654;
}

double MobilityPrediction::toGrad(double _rad)
{
    return _rad*180.0/3.141592654;
}

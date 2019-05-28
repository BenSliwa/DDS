#include "gpr.h"
#include "data/filehandler.h"

GPR::GPR(QObject *_parent) :
    QObject(_parent)
{

}

void GPR::init(const QString &_mno, const QString &_direction)
{
    QStringList lines = FileHandler::read("sdcard/gpr_" + _mno + "_" + _direction + ".txt");
    for(int i=0; i<lines.size(); i++)
    {
        QVector<double> values = FileHandler::parseCSV(lines.at(i));
        if(i==0)
        {
            m_min = values.at(0);
            m_max = values.at(1);
        }
        else
            m_data << QVector3D(values.at(0), values.at(1), values.at(2));
    }
}

QVector2D GPR::getCI(double _x)
{
    int s = m_data.size();
    for(int i=0; i<s; i++)
    {
        double current = m_data.at(i).x();

        QVector2D result(m_data.at(i).y(), m_data.at(i).z());
        if(i==0)
        {
            if(current>=_x)
                return result;
        }
        else if(i<s-1)
        {
            if(current>=_x)
                return result;
        }
        else
        {
            if(current<=_x)
                return result;
        }
    }
}

double GPR::limit(double _value)
{
    double value = _value;
    if(value<m_min)
        value = m_min;
    if(value>m_max)
        value = m_max;

    return value;
}

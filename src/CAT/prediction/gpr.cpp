#include "gpr.h"
#include "data/filehandler.h"

GPR::GPR()
{

}

void GPR::init(const QString &_file)
{
    QStringList lines = FileHandler::read(_file);
    foreach(QString line, lines)
    {
        QVector<double> values = FileHandler::parseCSV(line);
        m_data << QPointF(values.at(0), values.at(1));
    }
}

double GPR::getCI(double _x)
{
    int s = m_data.size();
    for(int i=0; i<s; i++)
    {
        double current = m_data.at(i).x();
        double result = m_data.at(i).y();
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

double GPR::limit(double _value, double _min, double _max)
{
    double value = _value;
    if(value<_min)
        value = _min;
    if(value>_max)
        value = _max;

    return value;
}

#ifndef GPR_H
#define GPR_H

#include <QVector>
#include <QPointF>

class GPR
{
public:
    GPR();

    void init(const QString &_file);
    double getCI(double _x);
    static double limit(double _value, double _min, double _max);


private:
    QVector<QPointF> m_data;

};

#endif // GPR_H

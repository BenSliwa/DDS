#ifndef GPR_H
#define GPR_H

#include <QObject>
#include <QVector>
#include <QVector3D>
#include <QVector2D>

class GPR : public QObject
{
    Q_OBJECT
public:
    GPR(QObject *_parent = 0);

    void init(const QString &_mno, const QString &_direction);
    QVector2D getCI(double _x);
    double limit(double _value);

private:
    QVector<QVector3D> m_data;
    double m_min;
    double m_max;

};

#endif // GPR_H

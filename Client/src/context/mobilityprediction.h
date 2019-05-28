#ifndef MOBILITYPREDICTION_H
#define MOBILITYPREDICTION_H

#include <QObject>
#include <QDebug>
#include <QVector>
#include <QVector2D>
#include <QVector3D>

class MobilityPrediction : public QObject
{
    Q_OBJECT
public:
    MobilityPrediction(QObject *_parent = 0);

    QVector3D predict(double _lat, double _lon, double _tau);

private:
    QVector<QVector2D> m_trace;
};

#endif // MOBILITYPREDICTION_H

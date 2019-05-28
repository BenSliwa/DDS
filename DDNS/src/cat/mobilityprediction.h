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

    void init(const QString &_scenario);

    QVector3D findNearestPosition(double _lat, double _lon);
    QVector3D predict(double _lat, double _lon, double _tau, double _velocity_mps);

    static double computeDistance(const QVector2D &_from, const QVector2D &_to);

private:
    static double toRad(double _grad);
    static double toGrad(double _rad);

private:
    QVector<QVector2D> m_trace;
};

#endif // MOBILITYPREDICTION_H

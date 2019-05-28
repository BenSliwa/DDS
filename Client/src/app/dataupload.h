#ifndef DATAUPLOAD_H
#define DATAUPLOAD_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QMap>
#include "context/context.h"


class DataUpload : public QObject
{
    Q_OBJECT
public:
    DataUpload(double _interval_s = 10, QObject *_parent = 0);

    virtual void transmit(const QVariantMap &_context);

protected:
    bool evaluateProbability(double _probability);
    double map(double _value, double _inMin, double _inMax, double _outMin, double _outMax);
    float predictThroughput(float _payload, const QVariantMap &_context, const QString &_mno, const QString &_direction);


private slots:
    void onTransmissionTimeout();


protected:
    QTimer m_timer;
    int m_ulCounter;
    int m_dlCounter;
};

#endif // DATAUPLOAD_H

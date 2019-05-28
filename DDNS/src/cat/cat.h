#ifndef CAT_H
#define CAT_H

#include "transmissionscheme.h"


class CAT : public TransmissionScheme
{
    Q_OBJECT
public:
    CAT(const Policy &_policy, const QString &_mno, const QString &_direction, const QString &_fileKey, QObject *_parent = 0);

    void clear(const QString &_scenario);
    void executeStep(const ContextEntry &_context);
    bool evaluateTransmission(const ContextEntry &_context, double _throughput);
    bool cat(double _value);
    bool pcat(double _value, double _prediction);

    double getCmSuccessRatio();

public:
    QString generateFileName(const QString &_scenario, int _run, int _seed);

signals:
    void transmitted(double _prediction, double _buffer_mb, double _deltaT_s, const QString &_context);


private:
    CellMap m_cellMap;
    ConnectivityMap m_connectivityMap;

    MobilityPrediction m_mobilityPrediction;

    Policy m_policy;
    QString m_mno;

    int m_cmSuccess;
    int m_cmFail;
};

#endif // CAT_H

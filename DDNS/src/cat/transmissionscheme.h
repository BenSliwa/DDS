#ifndef TRANSMISSIONSCHEME_H
#define TRANSMISSIONSCHEME_H

#include <QObject>
#include <QDebug>
#include "cellmap.h"
#include "mobilityprediction.h"
#include "connectivitymap.h"
#include "data/filehandler.h"

namespace POLICY_TYPE
{
    enum
    {
        PERIODIC,
        CAT,
        pCAT,
        ML_CAT,
        ML_pCAT
    };
}

struct Policy
{
    int type;
    double metricMin;
    double metricMax;
    double alpha;
    double tau;
    double gamma;

    QString getKey()
    {
        switch (type)
        {
            case POLICY_TYPE::PERIODIC: return "per";
            case POLICY_TYPE::CAT: return "cat";
            case POLICY_TYPE::pCAT: return "pcat";
            case POLICY_TYPE::ML_CAT: return "mlcat";
            case POLICY_TYPE::ML_pCAT: return "mlpcat";
            default: return "";
        }
    }
};

struct ContextEntry
{
    bool isValid;
    double timestamp_s;
    double lat;
    double lon;
    double rsrp;
    double rsrq;
    double sinr;
    double cqi;
    double ss;
    double ta;
    double velocity;
    bool connected;
    int cell;
    double f;
    QString raw;
};


class TransmissionScheme : public QObject
{
    Q_OBJECT
public:
    TransmissionScheme(const QString &_direction, const QString &_fileKey, QObject *_parent = 0);

public:
    ContextEntry parseContext(const QString &_line);
    QString serializeContext(const ContextEntry &_context);

    bool evaluateProbability(double _probability);
    double map(double _value, double _inMin, double _inMax, double _outMin, double _outMax);
    float predictThroughput(float payload, const ContextEntry &_context, const QString &_mno, const QString &_direction);
    virtual QString generateFileName(const QString &_scenario, int _run, int _seed) = 0;

    // accessors
    QString getDirection();

protected:
    QString m_direction;
    QString m_fileKey;

    double m_lastTransmission_s;
    double m_buffer_mb;


};

#endif // TRANSMISSIONSCHEME_H

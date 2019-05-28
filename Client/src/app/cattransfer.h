#ifndef CATTRANSFER_H
#define CATTRANSFER_H

#include "dataupload.h"
#include "context/cellmap.h"
#include "context/connectivitymap.h"
#include "context/mobilityprediction.h"

enum Scheme
{
    PERIODIC,
    CAT,
    pCAT
};

class CATTransfer : public DataUpload
{
    Q_OBJECT
public:
    CATTransfer(QObject *_parent = 0);

    virtual void transmit(const QVariantMap &_context);

    void uplink(const QVariantMap &_context);
    void downlink(const QVariantMap &_context);

    bool evaluateTransmissionProbability(double _metric, double _min, double _max);



signals:


private:
    CellMap m_cellMap;
    ConnectivityMap m_connectivityMap;

    double m_alpha;
    double m_metricMaxUL;
    double m_metricMaxDL;
    double m_lastULTransmission_s;
    double m_lastDLTransmission_s;

    int m_bufferUL;
    int m_bufferDL;
};

#endif // CATTRANSFER_H

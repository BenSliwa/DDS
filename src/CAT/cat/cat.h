#ifndef CAT_H
#define CAT_H

#include <QDebug>
#include "cellmap.h"
#include "prediction/gpr.h"

class CAT
{
public:
    CAT();

    void handleTraces(const QString &_folder, const QStringList &_files, const QString &_resultFolder);
    QString handleTrace(const QString &_file);
    bool evaluateTransmissionProbability(double _value, double _metricMin, double _metricMax);

    double map(double _value, double _inMin, double _inMax, double _outMin, double _outMax);
    double normRnd(double _mu, double _sigma);
private:
    CellMap m_cellMap;
    GPR m_gpr;


    double m_alpha;
    double m_metricMax;
    double m_lastTransmission_s;

    double m_result;
    int m_transmissions;
    double m_buffer_mb;
};

#endif // CAT_H

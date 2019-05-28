#ifndef DDS_H
#define DDS_H

#include <QObject>
#include <QDebug>
#include "dds/gpr.h"
#include "cat/cat.h"

struct DDSEntry
{
    double mu;
    double sigma;
    double result;
};

class DDS : public QObject
{
    Q_OBJECT
public:
    DDS(CAT *_cat, const QString &_mno, const QString &_direction, QObject *_parent = 0);

    void initRNG(int _seed);
    double handleTraces(const QString &_scenario, const QString &_folder, const QStringList &_files, const QString &_resultFolder);
    int handleTrace(const QString &_file, const QString &_scenario);
    QString ddsPrediction(const QString &_file);

public:
    static double normRnd(double _mu, double _sigma);

private slots:
    void onTransmitted(double _prediction, double _buffer_mb, double _deltaT_s, const QString &_context);

private:
    CAT *p_cat;
    GPR m_gpr;
    QString m_currentTrace;
    double m_result;
    int m_transmissions;
    int m_seed;
};

#endif // DDS_H

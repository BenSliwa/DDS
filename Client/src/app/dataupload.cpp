#include "dataupload.h"
#include "context/contextmanager.h"
#include <QTextStream>
#include "uimanager.h"
#include "settings/filehandler.h"
#include "uplink.h"
#include "downlink.h"
#include "context/prediction/TXPOWER_forest.h"
#include "context/prediction/o2_rf.h"
#include "context/prediction/tmobile_rf.h"
#include "context/prediction/vodafone_rf.h"
#include "context/prediction/o2_rf_dl.h"
#include "context/prediction/tmobile_rf_dl.h"
#include "context/prediction/vodafone_rf_dl.h"

DataUpload::DataUpload(double _interval_s, QObject *_parent) :
    QObject(_parent),
    m_ulCounter(0),
    m_dlCounter(0)
{
    qsrand(QDateTime::currentMSecsSinceEpoch() / 1000);

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTransmissionTimeout()));
    m_timer.start(_interval_s * 1000);
}

void DataUpload::transmit(const QVariantMap &_context)
{
    qDebug() << "DataUpload::transmit";

    Downlink *downlink = new Downlink(QString::number(m_dlCounter), _context, -1, this);
    Uplink *uplink = new Uplink(QString::number(m_ulCounter), _context, -1, this);

    m_ulCounter++;
    m_dlCounter++;
}

void DataUpload::onTransmissionTimeout()
{
    ContextManager *cm = ContextManager::getInstance();
    Context rawContext = cm->getContext();

    // transmit if connected
    if(rawContext.data.contains("isRegistered") && rawContext.data["isRegistered"].toInt()==1)
    {
        transmit(rawContext.data);
    }
    else
    {
    }
}

bool DataUpload::evaluateProbability(double _probability)
{
    double r = ((double) rand() / (RAND_MAX));
    if(r<_probability || _probability>=1)
        return true;
    return false;
}

double DataUpload::map(double _value, double _inMin, double _inMax, double _outMin, double _outMax)
{
    return (_value-_inMin) / (_inMax-_inMin) * (_outMax-_outMin) + _outMin;
}

float DataUpload::predictThroughput(float _payload, const QVariantMap &_context, const QString &_mno, const QString &_direction)
{
    float rsrp_dBm = _context["rsrp"].toDouble();
    float rsrq_dBm = _context["rsrq"].toDouble();
    float sinr_dB = _context["rssnr"].toDouble();
    float cqi = _context["cqi"].toDouble();
    float ss = _context["ss"].toDouble();
    float ta = _context["ta"].toDouble();
    float velocity_mps = _context["velocity"].toDouble();

    int cell = _context["ci"].toDouble();
    float f = _context["f"].toDouble();                                                            // TODO

    double throughputPrediction = 0;
    if(_direction=="ul")
    {
        if(_mno=="tmobile")
            throughputPrediction = tmobile_rf_prediction(_payload, rsrp_dBm, rsrq_dBm, sinr_dB, cqi, ss, ta, velocity_mps, cell, f);
        else if(_mno=="vodafone")
            throughputPrediction = vodafone_rf_prediction(_payload, rsrp_dBm, rsrq_dBm, sinr_dB, cqi, ss, ta, velocity_mps, cell, f);
        else if(_mno=="o2")
            throughputPrediction = o2_rf_prediction(_payload, rsrp_dBm, rsrq_dBm, sinr_dB, cqi, ss, ta, velocity_mps, cell, f);
    }
    else
    {
        if(_mno=="tmobile")
            throughputPrediction = tmobile_rf_dl_prediction(_payload, rsrp_dBm, rsrq_dBm, sinr_dB, cqi, ss, ta, velocity_mps, cell, f);
        else if(_mno=="vodafone")
            throughputPrediction = vodafone_rf_dl_prediction(_payload, rsrp_dBm, rsrq_dBm, sinr_dB, cqi, ss, ta, velocity_mps, cell, f);
        else if(_mno=="o2")
            throughputPrediction = o2_rf_dl_prediction(_payload, rsrp_dBm, rsrq_dBm, sinr_dB, cqi, ss, ta, velocity_mps, cell, f);
    }

    return throughputPrediction;
}




#include "cat.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <random>
#include <algorithm>

CAT::CAT(const Policy &_policy, const QString &_mno, const QString &_direction, const QString &_fileKey, QObject *_parent) :
    TransmissionScheme(_direction, _fileKey, _parent),
    m_policy(_policy),
    m_mno(_mno)
{
    m_connectivityMap.init(m_mno);
    m_cellMap.init(m_mno);
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void CAT::clear(const QString &_scenario)
{
    m_mobilityPrediction.init(_scenario);

    m_buffer_mb = 0;
    m_lastTransmission_s = 0;
    m_cmSuccess = 0;
    m_cmFail = 0;
}

void CAT::executeStep(const ContextEntry &_context)
{
    m_buffer_mb += 52429 / (1024.0 * 1024.0);

    ContextEntry context = _context;
    if(m_cellMap.hasEntry(context.cell))
        context.f = m_cellMap.getEntry(context.cell);

    // evaluate the transmission policies
    bool tx = false;
    double deltaT_s = fabs(context.timestamp_s-m_lastTransmission_s);
    double throughputPrediction = 0;
    if(context.connected && deltaT_s>120)
    {
        throughputPrediction = predictThroughput(m_buffer_mb, context, m_mno, m_direction);
        tx = true;
    }
    else if(context.connected && deltaT_s>=10)
    {
        throughputPrediction = predictThroughput(m_buffer_mb, context, m_mno, m_direction);
        tx = evaluateTransmission(context, throughputPrediction);
    }

    //
    if(tx)
    {
        emit transmitted(throughputPrediction, m_buffer_mb, deltaT_s, _context.raw);

        m_lastTransmission_s = context.timestamp_s;
        m_buffer_mb = 0;
    }
}

bool CAT::evaluateTransmission(const ContextEntry &_context, double _throughput)
{
    bool tx = false;
    if(m_policy.type==POLICY_TYPE::PERIODIC)
    {
        tx = true;
    }
    else if(m_policy.type==POLICY_TYPE::CAT)
    {
        tx = cat(_context.sinr);
    }
    else if(m_policy.type==POLICY_TYPE::pCAT)
    {
        QVector3D pos = m_mobilityPrediction.predict(_context.lat, _context.lon, m_policy.tau, _context.velocity);

        // the CM applies mean filtering - therefore the current measurement and the prediction should apply mean filtering
        ConnectivityMapEntry cm = m_connectivityMap.getEntry(pos.x(), pos.y());
        ConnectivityMapEntry cm0 = m_connectivityMap.getEntry(_context.lat, _context.lon);
        if(cm.isValid && cm0.isValid)
        {
            tx = pcat(cm0.sinr_dB, cm.sinr_dB);
            m_cmSuccess++;
        }
        else
        {
            // CAT fallback
            tx = cat(_context.sinr);
            m_cmFail++;

        }
    }
    else if(m_policy.type==POLICY_TYPE::ML_CAT)
    {
        tx = cat(_throughput);
    }
    else if(m_policy.type==POLICY_TYPE::ML_pCAT)
    {
        QVector3D pos = m_mobilityPrediction.predict(_context.lat, _context.lon, m_policy.tau, _context.velocity);

        // the CM applies mean filtering - therefore the current measurement and the prediction should apply mean filtering
        ConnectivityMapEntry cm = m_connectivityMap.getEntry(pos.x(), pos.y());
        if(cm.isValid)
        {
            ContextEntry context;
            context.isValid = true;
            context.rsrp = cm.rsrp_dBm;
            context.rsrq = cm.rsrq_dB;
            context.sinr = cm.sinr_dB;
            context.cqi = cm.cqi;
            context.ss = cm.ss;
            context.ta = cm.ta;
            context.velocity = cm.velocity_mps;
            context.cell = cm.ci;
            context.f = cm.f_MHz;

            double buffer_mb = m_buffer_mb + 0.05 * m_policy.tau;
            double prediction = predictThroughput(buffer_mb, context, m_mno, m_direction);

            tx = pcat(_throughput, prediction);
            m_cmSuccess++;
        }
        else
        {

            // ML-CAT fallback
            tx = cat(_throughput);
            m_cmFail++;
        }
    }

    return tx;
}

bool CAT::cat(double _value)
{
    double theta = map(_value, m_policy.metricMin, m_policy.metricMax, 0, 1);
    double p = powf(theta, m_policy.alpha);
    if(p<0)
        return false;
    return evaluateProbability(p);
}

bool CAT::pcat(double _value, double _prediction)
{
    double theta = map(_value, m_policy.metricMin, m_policy.metricMax, 0, 1);
    double deltaPhi = _prediction - _value;
    double z = 0;
    if(deltaPhi>0)
    {
        z = std::max( deltaPhi * (1-theta) * m_policy.gamma, 1.0 );
    }
    else
    {
        z = 1.0 / std::max( fabs( deltaPhi * theta * m_policy.gamma), 1.0 );
    }

    double p = powf(theta, m_policy.alpha * z);

    if(p<0)
        return false;
    return evaluateProbability(p);
}

double CAT::getCmSuccessRatio()
{
    if(m_policy.type==POLICY_TYPE::pCAT || m_policy.type==POLICY_TYPE::ML_pCAT)
        return (double)m_cmSuccess / (double)(m_cmSuccess + m_cmFail);
    return 1;
}

/*************************************
 *           PRIVATE METHODS         *
 ************************************/

QString CAT::generateFileName(const QString &_scenario, int _run, int _seed)
{
    QString file = m_fileKey;
    file.replace("$METRIC_MIN$", QString::number(m_policy.metricMin));
    file.replace("$METRIC_MAX$", QString::number(m_policy.metricMax));
    file.replace("$SCENARIO$", _scenario);
    file.replace("$DIRECTION$", m_direction);
    file.replace("$RUN$", QString::number(_run));
    file.replace("$MNO$", m_mno);
    file.replace("$ALPHA$", QString::number(m_policy.alpha));
    file.replace("$TAU$", QString::number(m_policy.tau));
    file.replace("$SEED$", QString::number(_seed));

    return file;
}



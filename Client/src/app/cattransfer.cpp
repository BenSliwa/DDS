#include "cattransfer.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "uimanager.h"
#include "uplink.h"
#include "downlink.h"
#include "settings/filehandler.h"

CATTransfer::CATTransfer(QObject *_parent) :
    DataUpload(1, _parent),
    m_alpha(6),
    m_metricMaxUL(30),
    m_metricMaxDL(30),
    m_lastULTransmission_s(0),
    m_lastDLTransmission_s(0),
    m_bufferUL(0),
    m_bufferDL(0)
{
    srand (time(NULL));

    m_cellMap.init("/sdcard/tmobile_cells_results.csv");
    m_cellMap.init("/sdcard/o2_cells_results.csv");
    m_cellMap.init("/sdcard/vodafone_cells_results.csv");

    m_connectivityMap.init();
}


void CATTransfer::transmit(const QVariantMap &_context)
{
    m_bufferUL += 52429;
    m_bufferDL += 52429;

    uplink(_context);
    downlink(_context);
}

void CATTransfer::uplink(const QVariantMap &_context)
{
    double timestamp_s = UiManager::getInstance()->getTimestamp();
    double deltaT_s = fabs(timestamp_s-m_lastULTransmission_s);

    if(deltaT_s>120)
    {
        Uplink *uplink = new Uplink(QString::number(m_ulCounter), _context, m_bufferUL, this);
        Q_UNUSED(uplink);

        m_bufferUL = 0;
        m_ulCounter++;
        m_lastULTransmission_s = timestamp_s;
    }
    else if(deltaT_s>=10)
    {
        float payload_mb = m_bufferUL / (1024 * 1024);

        int ci = _context["ci"].toDouble();
        float f_MHz = 900;
        if(m_cellMap.hasEntry(ci))
            f_MHz = m_cellMap.getEntry(ci);
        QVariantMap context = _context;
        context["f"] = f_MHz;

        double metric = 0;
        metric = predictThroughput(payload_mb, context, UiManager::getCarrier(), "ul");


        //double metric = _context["rssnr"].toDouble();
        //bool transmit = evaluateTransmissionProbability(metric, 0, m_metricMaxUL);
        //transmit = true; // periodic
        bool transmit = evaluateTransmissionProbability(_context["rssnr"].toDouble(), 0, m_metricMaxUL);

        if(transmit)
        {
            Uplink *uplink = new Uplink(QString::number(m_ulCounter), _context, m_bufferUL, this);
            Q_UNUSED(uplink);

            m_bufferUL = 0;
            m_ulCounter++;
            m_lastULTransmission_s = timestamp_s;
        }
    }
}
void CATTransfer::downlink(const QVariantMap &_context)
{
    double timestamp_s = UiManager::getInstance()->getTimestamp();
    double deltaT_s = fabs(timestamp_s-m_lastDLTransmission_s);

    if(deltaT_s>120)
    {
        Downlink *downlink = new Downlink(QString::number(m_dlCounter), _context, m_bufferDL, this);
        Q_UNUSED(downlink);

        m_bufferDL = 0;
        m_dlCounter++;
        m_lastDLTransmission_s = timestamp_s;
    }
    else if(deltaT_s>=10)
    {
        float payload_mb = m_bufferUL / (1024 * 1024);

        int ci = _context["ci"].toDouble();
        float f_MHz = 900;
        if(m_cellMap.hasEntry(ci))
            f_MHz = m_cellMap.getEntry(ci);
        QVariantMap context = _context;
        context["f"] = f_MHz;

        double metric = 0;
        metric = predictThroughput(payload_mb, context, UiManager::getCarrier(), "dl");


        //double metric = _context["rssnr"].toDouble();
        //bool transmit = evaluateTransmissionProbability(metric, 0, m_metricMaxDL);
        //transmit = true; // periodic

        bool transmit = evaluateTransmissionProbability(_context["rssnr"].toDouble(), 0, m_metricMaxDL);

        if(transmit)
        {
            Downlink *downlink = new Downlink(QString::number(m_dlCounter), _context, m_bufferDL, this);
            Q_UNUSED(downlink);

            m_bufferDL = 0;
            m_dlCounter++;
            m_lastDLTransmission_s = timestamp_s;
        }
    }
}


bool CATTransfer::evaluateTransmissionProbability(double _metric, double _min, double _max)
{
    double p = powf(_metric/_max, m_alpha);
    if(p<0)
        return false;

    double r = ((double) rand() / (RAND_MAX));

    qDebug() << "CATTransfer::evaluateTransmissionProbability" << _metric << p << r;

    if(r<p)
        return true;
    return false;
}



#include "transmissionscheme.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <random>
#include <QDateTime>
#include "prediction/o2_rf.h"
#include "prediction/o2_rf_dl.h"
#include "prediction/tmobile_rf.h"
#include "prediction/tmobile_rf_dl.h"
#include "prediction/vodafone_rf.h"
#include "prediction/vodafone_rf_dl.h"

TransmissionScheme::TransmissionScheme(const QString &_direction, const QString &_fileKey, QObject *_parent) :
    QObject(_parent),
    m_direction(_direction),
    m_fileKey(_fileKey),
    m_lastTransmission_s(0)
{
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/




/*************************************
 *          PROTECTED METHODS        *
 ************************************/

ContextEntry TransmissionScheme::parseContext(const QString &_line)
{
    ContextEntry entry;
    QVector<double> values  = FileHandler::parseCSV(_line);
    if(values.size()>15)
    {
        entry.timestamp_s = values.at(0);
        entry.lat = values.at(3);
        entry.lon = values.at(4);
        entry.rsrp = values.at(10);
        entry.rsrq = values.at(11);
        entry.sinr = values.at(12);
        entry.cqi = values.at(13);
        entry.ss = values.at(14);
        entry.ta = values.at(15);
        entry.velocity = values.at(6);
        entry.connected = values.at(9);
        entry.cell = values.at(16);
    }
    else
        entry.isValid = false;

    return entry;
}

QString TransmissionScheme::serializeContext(const ContextEntry &_context)
{
    QString data;
    QTextStream stream(&data);



    return data;
}


bool TransmissionScheme::evaluateProbability(double _probability)
{
    double r = ((double) rand() / (RAND_MAX));
    if(r<_probability || _probability>=1)
        return true;
    return false;
}

float TransmissionScheme::predictThroughput(float payload, const ContextEntry &_context, const QString &_mno, const QString &_direction)
{
    double throughputPrediction = 0;
    if(_direction=="ul")
    {
        if(_mno=="tmobile")
            throughputPrediction = tmobile_rf_prediction(payload, _context.rsrp, _context.rsrq, _context.sinr, _context.cqi, _context.ss, _context.ta, _context.velocity, _context.cell, _context.f);
        else if(_mno=="vodafone")
            throughputPrediction = vodafone_rf_prediction(payload, _context.rsrp, _context.rsrq, _context.sinr, _context.cqi, _context.ss, _context.ta, _context.velocity, _context.cell, _context.f);
        else if(_mno=="o2")
            throughputPrediction = o2_rf_prediction(payload, _context.rsrp, _context.rsrq, _context.sinr, _context.cqi, _context.ss, _context.ta, _context.velocity, _context.cell, _context.f);
    }
    else
    {
        if(_mno=="tmobile")
            throughputPrediction = tmobile_rf_dl_prediction(payload, _context.rsrp, _context.rsrq, _context.sinr, _context.cqi, _context.ss, _context.ta, _context.velocity, _context.cell, _context.f);
        else if(_mno=="vodafone")
            throughputPrediction = vodafone_rf_dl_prediction(payload, _context.rsrp, _context.rsrq, _context.sinr, _context.cqi, _context.ss, _context.ta, _context.velocity, _context.cell, _context.f);
        else if(_mno=="o2")
            throughputPrediction = rf_o2_dl_prediction(payload, _context.rsrp, _context.rsrq, _context.sinr, _context.cqi, _context.ss, _context.ta, _context.velocity, _context.cell, _context.f);
    }

    return throughputPrediction;
}

double TransmissionScheme::map(double _value, double _inMin, double _inMax, double _outMin, double _outMax)
{
    return (_value-_inMin) / (_inMax-_inMin) * (_outMax-_outMin) + _outMin;
}

QString TransmissionScheme::getDirection()
{
    return m_direction;
}

#include "cat.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <random>
#include <QDateTime>
#include "data/filehandler.h"
#include "prediction/forest.h"

CAT::CAT() :
    m_alpha(6),
    m_metricMax(30),
    m_lastTransmission_s(-100),
    m_result(0),
    m_transmissions(0)
{
    srand(time(NULL));

    m_cellMap.init("tmobile_cells_results.csv");
    m_gpr.init("gpr_model.txt");
}

void CAT::handleTraces(const QString &_folder, const QStringList &_files, const QString &_resultFolder)
{
    QString times;
    QTextStream stream(&times);
    foreach(QString file, _files)
    {
        qint64 start = QDateTime::currentMSecsSinceEpoch();
        QString result = handleTrace(_folder + file);
        FileHandler::write(result, _resultFolder + file);
        qint64 end = QDateTime::currentMSecsSinceEpoch();

        stream << end-start << "\n";
    }

    qDebug() << "CAT::handleTrace" << m_result/(double)m_transmissions << m_result << m_transmissions;

}

QString CAT::handleTrace(const QString &_file)
{
    QString result;
    QTextStream stream(&result);

    QStringList lines = FileHandler::read(_file);
    foreach(QString line, lines)
    {
        m_buffer_mb += 0.05;

        QVector<double> values  = FileHandler::parseCSV(line);

        int cell = values.at(16);
        if(m_cellMap.hasEntry(cell))
        {
            double timestamp_s = values.at(0);
            double payload = m_buffer_mb;
            double rsrp = values.at(10);
            double rsrq = values.at(11);
            double sinr = values.at(12);
            double cqi = values.at(13);
            double ss = values.at(14);
            double ta = values.at(15);
            double velocity = values.at(6);
            bool connected = values.at(9);

            double f = m_cellMap.getEntry(cell);



            // evaluate the transmission policies
            bool tx = false;
            double deltaT_s = fabs(timestamp_s-m_lastTransmission_s);
            if(connected && deltaT_s>120)
            {
                tx = true;
            }
            else if(connected && deltaT_s>=10)
            {

                tx = evaluateTransmissionProbability(sinr, 0, 30);
                //bool transmit = evaluateTransmissionProbability(rsrp, -120, -70);

                double throughputPrediction = prediction(payload, rsrp, rsrq, sinr, cqi, ss, ta, velocity, cell, f);
                tx = evaluateTransmissionProbability(throughputPrediction, 0, m_metricMax);

                tx = 1; // periodic
            }


            //
            if(tx)
            {
                double mu = prediction(payload, rsrp, rsrq, sinr, cqi, ss, ta, velocity, cell, f);
                double ci = fabs(mu - m_gpr.getCI(mu));
                double sigma = (mu-ci)/1.96;

                double sample = normRnd(mu, sigma);
                double result = GPR::limit(sample, 0.2770, 40.0600);

                double payload = m_buffer_mb;
                stream << result << "," << payload << "\n";

                m_result += result;
                m_transmissions++;

                m_lastTransmission_s = timestamp_s;
                m_buffer_mb = 0;
            }
        }
    }

    return result;
}

bool CAT::evaluateTransmissionProbability(double _value, double _metricMin, double _metricMax)
{
    double theta = map(_value, _metricMin, _metricMax, 0, 1);
    double p = powf(theta, m_alpha);
    if(p<0)
        return false;

    double r = ((double) rand() / (RAND_MAX));
    if(r<p)
        return true;
    return false;
}

double CAT::map(double _value, double _inMin, double _inMax, double _outMin, double _outMax)
{
    return (_value-_inMin) / (_inMax-_inMin) * (_outMax-_outMin) + _outMin;
}


double CAT::normRnd(double _mu, double _sigma)
{
    static double n2 = 0.0;
    static int n2_cached = 0;
    if (!n2_cached)
    {
        double x, y, r;
        do
        {
            x = 2.0*rand()/RAND_MAX - 1;
            y = 2.0*rand()/RAND_MAX - 1;

            r = x*x + y*y;
        }
        while (r == 0.0 || r > 1.0);
        {
            double d = sqrt(-2.0*log(r)/r);
            double n1 = x*d;
            n2 = y*d;
            double result = n1*_sigma + _mu;
            n2_cached = 1;
            return result;
        }
    }
    else
    {
        n2_cached = 0;
        return n2*_sigma + _mu;
    }
}


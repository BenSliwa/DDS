#include "transmissionmanager.h"
#include "prediction/TXPOWER_forest.h"
#include <QDateTime>


TransmissionManager::TransmissionManager(QObject *_parent) :
    QObject(_parent),
    m_address("129.217.211.17"),
    m_port(8237),
    p_context(ContextManager::getInstance())
{

}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void TransmissionManager::start()
{
    m_uplink = 0;
    m_downlink = 0;

    p_context->start();
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    m_timer.start(1000);
    onTimeout();
}

void TransmissionManager::stop()
{
    m_timer.stop();
    this->deleteLater();
}

void TransmissionManager::addTransmissionScheme(CAT *_cat)
{
    m_schemes << _cat;
    connect(_cat, SIGNAL(transmitted(double,double,double,QString)), this, SLOT(onTransmitted(double,double,double,QString)));
}

/*************************************
 *           PRIVATE METHODS         *
 ************************************/

void TransmissionManager::updateTransmissionSchemes(const Context &_context)
{
    ContextEntry entry;
    entry.isValid = true;
    entry.timestamp_s = _context.get("timestamp");
    entry.rsrp = _context.get("rsrp");
    entry.rsrq = _context.get("rsrq");
    entry.sinr = _context.get("rssnr");
    entry.cqi = _context.get("cqi");
    entry.ss = _context.get("ss");
    entry.ta = _context.get("ta");
    entry.cell = _context.get("ci");
    entry.connected = _context.get("isRegistered");

    entry.lat = _context.get("latitude");
    entry.lon = _context.get("longitude");
    entry.velocity = _context.get("velocity");
    entry.raw = p_context->serializeContextEntry(_context);

    // evaluate all transmission schemes
    foreach (CAT *cat, m_schemes)
        cat->executeStep(entry);

    //qDebug() << entry.timestamp_s << entry.lat << entry.lon << entry.rsrp << entry.sinr;
}

/*************************************
 *            PRIVATE SLOTS          *
 ************************************/

void TransmissionManager::onTimeout()
{
    updateTransmissionSchemes(p_context->getContext());
}

void TransmissionManager::onTransmitted(double _prediction, double _buffer_mb, double _deltaT_s, const QString &_context)
{
    CAT *cat = qobject_cast<CAT*>(QObject::sender());
    if(cat)
    {
        int bytes = _buffer_mb * (1024.0 * 1024.0);
        QString dir = cat->getDirection();

        Transmission *transmission = 0;
        if(dir=="ul")
            transmission = new Uplink(QString::number(m_uplink++), this);
        else
            transmission = new Downlink(QString::number(m_downlink++), this);

        m_transmissions[transmission] = _context;
        connect(transmission, SIGNAL(success(double,double,double,QString)), this, SLOT(onSuccess(double,double,double,QString)));
        transmission->start(bytes, m_address, m_port);
    }
}

void TransmissionManager::onSuccess(double _throughput_mbits, double _payload_mb, double _rtt_ms, const QString &_direction)
{
    Transmission *transmission = qobject_cast<Transmission*>(QObject::sender());
    if(transmission)
    {
        if(m_transmissions.contains(transmission))
        {
            QString context = m_transmissions[transmission];
            m_transmissions.remove(transmission);

            QString log;
            QTextStream stream(&log);
            stream << context << "," << transmission->getId() << "," << _payload_mb << "," << _throughput_mbits << "," << _rtt_ms;

            QString path = "/sdcard/CAT/" + p_context->getSession();
            if(_direction=="ul")
            {
                QVector<double> entry = FileHandler::parseCSV(context);
                float rsrp = entry.at(10);
                float rsrq = entry.at(11);
                float rssnr = entry.at(12);
                float velocity_kmh = entry.at(6) * 3.6;
                QVector<float> features = QVector<float>() << rsrp << rsrq << rssnr << velocity_kmh << _payload_mb;
                float txPower_dBm = txpower_predict(features.constData());
                stream << "," << txPower_dBm;

                path += "_ul.txt";
            }
            else
            {
                path += "_dl.txt";
            }


            FileHandler::append(log, path);
        }
    }

    emit transmitted(_throughput_mbits, _direction);
}


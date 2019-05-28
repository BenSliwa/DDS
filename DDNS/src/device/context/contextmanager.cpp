#include "contextmanager.h"
#include "data/filehandler.h"
#include "networkcontext.h"
#include <QDateTime>

ContextManager::ContextManager(QObject *_parent) :
    QObject(_parent),
    p_network(NetworkContext::getInstance()),
    p_mobility(MobilityContext::getInstance()),
    m_counter(0),
    m_active(false),
    m_startTime_ms(0)
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    m_timer.start(100);
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

ContextManager* ContextManager::getInstance()
{
    static ContextManager instance;
    return &instance;
}

void ContextManager::start()
{
    m_active = true;
    m_counter = -1;
    m_startTime_ms = 0;
    m_startTime_ms = getTimestamp();

    m_context.data.clear();
    m_context.data["distance"] = 0;
    m_context.data["timestamp"] = getTimestamp() / 1000.0;
}

void ContextManager::stop()
{
    m_active = false;
    m_counter = -1;
}

qint64 ContextManager::getTimestamp()
{
    return QDateTime::currentMSecsSinceEpoch() - m_startTime_ms;
}

QString ContextManager::getSession()
{
    int timestamp = m_startTime_ms / 1000.0;
    return QString::number(timestamp, 'g', 10) + "_" + NetworkContext::getInstance()->getCarrier();
}

QString ContextManager::serializeContextEntry(const Context &_context)
{
    QString data;
    QTextStream stream(&data);
    stream << qSetRealNumberPrecision(8);
    stream << _context.getString("timestamp", 2) << ",";
    stream << _context.getString("rawTimestamp", 0) << ",";
    stream << _context.getString("distance", 2) << ",";

    // mobility context
    stream << _context.getString("latitude", 8) << ",";
    stream << _context.getString("longitude", 8) << ",";
    stream << _context.getString("altitude", 2) << ",";
    stream << _context.getString("velocity", 2) << ",";
    stream << _context.getString("acceleration", 2) << ",";
    stream << _context.getString("direction", 2) << ",";

    stream << _context.getString("isRegistered", 0) << ",";
    stream << _context.getString("rsrp", 2) << ",";
    stream << _context.getString("rsrq", 2) << ",";
    stream << _context.getString("rssnr", 2) << ",";
    stream << _context.getString("cqi", 2) << ",";
    stream << _context.getString("ss", 2) << ",";
    stream << _context.getString("ta", 2) << ",";
    stream << _context.getString("ci", 0) << ",";
    stream << _context.getString("pci", 0);

    return data;
}

void ContextManager::logContext()
{
    QString data = serializeContextEntry(m_context);
    QString path = "/sdcard/CAT/" + getSession() + "_context.txt";

    FileHandler::append(data, path);
}

void ContextManager::logCells()
{
    QString data;
    QTextStream stream(&data);
    QVariantList cells = p_network->getCells();

    if(cells.size()>0)
    {
        for(int i=0; i<cells.size(); i++)
        {
            QVariantMap cell = cells.at(i).toMap();
            stream << serializeContextEntry(createContextEntry(cell));
            if(i<cells.size()-1)
                stream << "\n";
        }

        QString path = "/sdcard/CAT/" + getSession() + "_cells.txt";
        FileHandler::append(data, path);
    }
}

Context ContextManager::createContextEntry(const QVariantMap &_data)
{
    Context context;
    context.data = _data;
    context.data["timestamp"] = getTimestamp() / 1000.0;
    context.data["rawTimestamp"] = QDateTime::currentMSecsSinceEpoch() / 1000.0;
    context.add(p_mobility->getData());

    // compute distance and acceleration
    if(m_context.data.size()>0)
    {
        double velocity_mps = m_context.data["velocity"].toDouble();
        double timeDelta_s = context.data["timestamp"].toDouble() - m_context.data["timestamp"].toDouble();
        double distance_m = velocity_mps * timeDelta_s;

        if(timeDelta_s>0 && velocity_mps!=0)
            context.data["acceleration"] = (context.data["velocity"].toDouble() - velocity_mps) / timeDelta_s;
        else
            context.data["acceleration"] = 0;
        context.data["distance"] = m_context.data["distance"].toDouble() + distance_m;
    }

    return context;
}

Context ContextManager::getContext()
{
    return m_context;
}

/*************************************
 *            PRIVATE SLOTS          *
 ************************************/

void ContextManager::onTimeout()
{
    m_counter++;

    Context context = createContextEntry(p_network->getData());
    if(m_active && m_counter%10==0)
    {
        m_context = context;

        logContext();
        logCells();
        m_counter = 0;
    }
}

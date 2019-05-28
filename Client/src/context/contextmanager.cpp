#include "contextmanager.h"
#include "uimanager.h"
#include <QTextStream>
#include <QGeoCoordinate>
#include "settings/filehandler.h"


ContextManager::ContextManager(QObject *_parent) :
    QObject(_parent),
    m_counter(-1),
    m_active(false)
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onUpdated()));
    m_timer.start(100);
}

ContextManager* ContextManager::getInstance()
{
    static ContextManager instance;
    return &instance;
}

void ContextManager::start()
{
    m_active = true;
    m_counter = -1;

    m_context.data.clear();
    m_context.data["distance"] = 0;
    m_context.data["timestamp"] = UiManager::getInstance()->getTimestamp();
}

void ContextManager::stop()
{
    m_active = false;
}

Context ContextManager::createContextEntry(const QVariantMap &_data)
{
    Context context;
    context.data = _data;
    context.data["timestamp"] = UiManager::getInstance()->getTimestamp();
    context.data["rawTimestamp"] = UiManager::getInstance()->getRawTimestamp();
    context.add(m_mobilityContext.getData());

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

QString ContextManager::getCarrier()
{
    return m_networkContext.getCarrier();
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
    UiManager *ui = UiManager::getInstance();
    QString path = ui->getDataPath() + ui->getSession() + "_context.txt";

    //qDebug() << path;

    FileHandler::append(data, path);

    //qDebug() << data;
}

void ContextManager::logCells()
{
    QString data;
    QTextStream stream(&data);
    QVariantList cells = m_networkContext.getCells();

    if(cells.size()>0)
    {
        for(int i=0; i<cells.size(); i++)
        {
            QVariantMap cell = cells.at(i).toMap();
            stream << serializeContextEntry(createContextEntry(cell));
            if(i<cells.size()-1)
                stream << "\n";
        }

        UiManager *ui = UiManager::getInstance();
        QString path = ui->getDataPath() + ui->getSession() + "_cells.txt";
        FileHandler::append(data, path);
    }
}

void ContextManager::onUpdated()
{
    m_counter++;

    Context context = createContextEntry(m_networkContext.getData());
    emit updated(context.data);

    if(m_active && m_counter%10==0)
    {
        m_context = context;

        logContext();
        logCells();
        m_counter = 0;
    }
}

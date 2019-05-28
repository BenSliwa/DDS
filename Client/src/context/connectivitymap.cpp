#include "connectivitymap.h"
#include "uimanager.h"
#include "settings/filehandler.h"

ConnectivityMap::ConnectivityMap(QObject *_parent) :
    QObject(_parent)
{


}

void ConnectivityMap::init()
{
    QString carrier = UiManager::getCarrier();
    QStringList lines = FileHandler::read("/sdcard/" + carrier + "_cm_online.txt");

    for(int i=0; i<lines.size(); i++)
    {
        QVector<double> line = FileHandler::parseCSV(lines.at(i));
        if(i==0 && line.size()==6)
        {
            m_origin = QVector2D(line.at(0), line.at(2));
            m_inc = QVector2D(line.at(4), line.at(5));
        }
        else if(i>0 && line.size()==11)
        {
            int x = (int)line.at(0);
            int y = (int)line.at(1);

            ConnectivityMapEntry entry;
            entry.isValid = true;
            entry.rsrp_dBm = line.at(2);
            entry.rsrq_dB = line.at(3);
            entry.sinr_dB = line.at(4);
            entry.cqi = line.at(5);
            entry.ss = line.at(6);
            entry.ta = line.at(7);
            entry.velocity_mps = line.at(8);
            entry.ci = (int)line.at(9);
            entry.f_MHz = line.at(10);


            m_entries[QString::number(x) + "," + QString::number(y)] = entry;
        }
    }

    qDebug() << "ConnectivityMap::init" << m_entries.size();
}

ConnectivityMapEntry ConnectivityMap::getEntry(double _lat, double _lon)
{
    ConnectivityMapEntry entry;
    entry.isValid = false;

    int x = ((_lon - m_origin.y()) / m_inc.y()) + 2; // lon
    int y = ((_lat - m_origin.x()) / m_inc.x()) + 2; // lat
    QString key = QString::number(x) + "," + QString::number(y);

    if(m_entries.contains(key))
        entry = m_entries[key];

    return entry;
}


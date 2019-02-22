#include "cellmap.h"
#include "data/filehandler.h"


CellMap::CellMap()
{

}

void CellMap::init(const QString &_file)
{
    QStringList lines = FileHandler::read(_file);
    foreach(QString line, lines)
    {
        QVector<double> values = FileHandler::parseCSV(line);
        m_frequencies[values.at(0)] = values.at(4);
    }

}

bool CellMap::hasEntry(int _id)
{
    return m_frequencies.contains(_id);
}

double CellMap::getEntry(int _id)
{
    return m_frequencies[_id];
}

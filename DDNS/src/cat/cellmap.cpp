#include "cellmap.h"
#include "data/filehandler.h"


CellMap::CellMap(QObject *_parent) :
    QObject(_parent)
{

}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void CellMap::init(const QString &_mno)
{
    QString path;
    #ifdef Q_OS_ANDROID
        path = "/sdcard/" + _mno + "_cells_results.csv";
    #else
        path = "sdcard/" + _mno + "_cells_results.csv";
    #endif

    QStringList lines = FileHandler::read(path);
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

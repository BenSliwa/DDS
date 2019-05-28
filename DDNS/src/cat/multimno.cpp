#include "multimno.h"

MultiMNO::MultiMNO(const QString &_direction, const QString &_fileKey, QObject *_parent) :
    TransmissionScheme(_direction, _fileKey, _parent)
{
    QStringList mnos = QStringList() << "tmobile" << "vodafone" << "o2";
    foreach(QString mno, mnos)
    {
        //
        ConnectivityMap *cm = new ConnectivityMap(this);
        cm->init(mno);
        m_cms[mno] = cm;

        //
        CellMap *cell = new CellMap(this);
        cell->init(mno + "_cells_results.csv");
        m_cells[mno] = cell;

        //
        GPR *gpr = new GPR(this);
        gpr->init(mno, m_direction);
        m_gprs[mno] = gpr;
    }

}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void MultiMNO::addPolicy(const QString &_mno, const Policy &_policy)
{
    m_policies[_mno] = _policy;
}

QString MultiMNO::handleTrace(const QString &_file)
{
    QString result;
    QTextStream stream(&result);
    m_lastTransmission_s = 0;

    QStringList lines = FileHandler::read(_file);




    return result;
}


/*************************************
 *          PROTECTED METHODS        *
 ************************************/

QString MultiMNO::generateFileName(const QString &_scenario, int _run)
{
    QString file = m_fileKey;


    return file;
}

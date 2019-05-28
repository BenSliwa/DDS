#ifndef MULTIMNO_H
#define MULTIMNO_H

#include "transmissionscheme.h"
#include "dds/gpr.h"

/*  TODO
 *  - for a given folder, find all mno-tuples: tmobile, vodafone and o2, where the time delta is minimized
 *
 *
 *
 *
 *
 */


class MultiMNO : public TransmissionScheme
{
    Q_OBJECT
public:
    MultiMNO(const QString &_direction, const QString &_fileKey, QObject *_parent = 0);

    void addPolicy(const QString &_mno, const Policy &_policy);
    virtual QString handleTrace(const QString &_file);

protected:
    QString generateFileName(const QString &_scenario, int _run);

private:
    QMap<QString, Policy> m_policies;
    QMap<QString, ConnectivityMap*> m_cms;
    QMap<QString, CellMap*> m_cells;
    QMap<QString, GPR*> m_gprs;
};

#endif // MULTIMNO_H

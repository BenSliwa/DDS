#include <QtWidgets/QApplication>
#include <QDebug>
#include <QDir>
#include <QDateTime>
#include "data/dataparser.h"
#include "cat/cat.h"
#include "ui/uimanager.h"
#include "ui/uimanagerandroid.h"
#include "dds/dds.h"


void evaluate(const QStringList &_scenarios, const QString &_mno, const QString &_direction)
{
    QString key = "$MNO$_$DIRECTION$_$SCENARIO$_$RUN$.txt";

    Policy policy{POLICY_TYPE::ML_CAT, 0, 30, 6, 10, 1};
    CAT cat(policy, _mno, _direction, key);

    foreach(QString scenario, _scenarios)
    {
        QString folder = "data/" + scenario + "/";

        QDir directory(folder);
        QStringList files = directory.entryList(QStringList() << "*_" + _mno + "_context.txt", QDir::Files);
        QString results = "results/";


        DDS dds(&cat, _mno, _direction);
        qDebug() << dds.handleTraces(scenario, folder, files, results);
    }


}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QStringList scenarios = QStringList() << "suburban" << "highway";

    evaluate(scenarios, "tmobile", "ul");

    qDebug() << "done";

    return 0;
}

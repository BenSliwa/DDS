#include "dds.h"
#include <QDateTime>


DDS::DDS(CAT *_cat, const QString &_mno, const QString &_direction, QObject *_parent) :
    QObject(_parent),
    p_cat(_cat),
    m_result(0),
    m_transmissions(0)
{
    initRNG(-1);
    m_gpr.init(_mno, _direction);

    connect(p_cat, SIGNAL(transmitted(double,double,double,QString)), this, SLOT(onTransmitted(double,double,double,QString)));
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void DDS::initRNG(int _seed)
{
    m_seed = _seed;

    srand(m_seed);
    if(m_seed==-1)
        srand(time(0));
}

double DDS::handleTraces(const QString &_scenario, const QString &_folder, const QStringList &_files, const QString &_resultFolder)
{
    m_result = 0;
    m_transmissions = 0;

    QString times;
    QTextStream stream(&times);
    for(int i=0; i<_files.size(); i++)
    {
        QString file = _files.at(i);
        qint64 start = QDateTime::currentMSecsSinceEpoch();

        m_currentTrace = "";
        int lines = handleTrace(_folder + file, _scenario);
        //QString result = ddsPrediction(_folder + file);

        QString fileName = p_cat->generateFileName(_scenario, i, m_seed);
        FileHandler::write(m_currentTrace, _resultFolder + fileName);
        qint64 end = QDateTime::currentMSecsSinceEpoch();

        stream << end-start << "," << lines << "\n";
    }


    double result = m_result/m_transmissions;

    //qDebug() << "DDS::handleTraces" << result << p_cat->getCmSuccessRatio();



    FileHandler::write(times, _resultFolder + "computation_times.txt");

    return result;
}

int DDS::handleTrace(const QString &_file, const QString &_scenario)
{
    p_cat->clear(_scenario);

    QStringList lines = FileHandler::read(_file);
    int s = lines.size();
    foreach(QString line, lines)
        p_cat->executeStep(p_cat->parseContext(line));

    return s;
}

QString DDS::ddsPrediction(const QString &_file)
{
    /*
     *     QString result;
    QTextStream stream(&result);
    QStringList lines = FileHandler::read(_file);

    foreach(QString line, lines)
    {

        QVector<double> values  = FileHandler::parseCSV(line);
        ContextEntry context = parseContext(line);
        if(m_cellMap.hasEntry(context.cell))
        {
            double payload = values.at(19);
            context.f = m_cellMap.getEntry(context.cell);
            double realThroughput = values.at(20);

            double throughputPrediction = predictThroughput(payload, context, m_mno, m_direction);

            QVector2D gprEntry = m_gpr.getCI(throughputPrediction);
            double mu = gprEntry.x();
            double sigma = gprEntry.y();
            double sample = DDS::normRnd(mu, sigma);
            double result = m_gpr.limit(sample);

            //result = sample;

            stream << realThroughput << "," << result << "," << throughputPrediction << "," << mu << "\n";

        }
        else
            qDebug() << "no entry for" << context.cell;
    }

    return result;
*/
}

/*************************************
 *           PRIVATE METHODS         *
 ************************************/

double DDS::normRnd(double _mu, double _sigma)
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

/*************************************
 *            PRIVATE SLOTS          *
 ************************************/

void DDS::onTransmitted(double _prediction, double _buffer_mb, double _deltaT_s, const QString &_context)
{
    QVector2D gprEntry = m_gpr.getCI(_prediction);
    double mu = gprEntry.x();
    double sigma = gprEntry.y();
    double sample = DDS::normRnd(mu, sigma);
    double result = m_gpr.limit(sample);

    QTextStream stream(&m_currentTrace);
    stream << result << "," << _buffer_mb << "," << _deltaT_s << "," << "\n";

    m_result += result;
    m_transmissions++;
}


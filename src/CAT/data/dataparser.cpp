#include "dataparser.h"
//#include "prediction/forest.h"
#include "filehandler.h"

DataParser::DataParser()
{

}

void DataParser::parseFile(const QString &_path)
{
    /*
    QStringList lines = FileHandler::read(_path);

    qDebug() << "DataParser::parseFile" << lines.size();
    for(int i=0; i<lines.size(); i++)
    {
        QString line = lines.at(i);
        if(i==0)
            m_header = parseHeader(line);
        else
        {
            QVector<double> values  = FileHandler::parseCSV(line);

            double throughput = values.at(m_header.indexOf("throughput"));

            double payload = values.at(m_header.indexOf("payload"));
            double rsrp = values.at(m_header.indexOf("rsrp"));
            double rsrq = values.at(m_header.indexOf("rsrq"));
            double sinr = values.at(m_header.indexOf("sinr"));
            double cqi = values.at(m_header.indexOf("cqi"));
            double ss = values.at(m_header.indexOf("ss"));
            double ta = values.at(m_header.indexOf("ta"));
            double velocity = values.at(m_header.indexOf("velocity"));
            double cell = values.at(m_header.indexOf("cell"));
            double f = values.at(m_header.indexOf("f"));

            double result = prediction(payload, rsrp, rsrq, sinr, cqi, ss, ta, velocity, cell, f);

            qDebug() << result << throughput << payload << rsrp << rsrq << sinr << cqi << ss << ta << velocity << cell << f;

        }
    }*/
}

QStringList DataParser::parseHeader(const QString &_line)
{
    return _line.split(",");
}


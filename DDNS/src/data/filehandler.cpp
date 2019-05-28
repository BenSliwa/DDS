#include "filehandler.h"
#include <QDir>
#include <QFile>
#include <QTextStream>

FileHandler::FileHandler()
{

}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/


QStringList FileHandler::read(const QString &_file)
{
    QStringList lines;

    QFile file(_file);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        while(!in.atEnd())
            lines << in.readLine();

        file.close();
    }
    else
        qDebug() << "FileHandler::read failed" <<  _file;

    return lines;
}

void FileHandler::write(const QString &_data, const QString &_file)
{
    QFile file(_file);
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        stream << _data;
        file.close();
    }
    else
        qDebug() << "FileHandler::write failed" << _file;
}

void FileHandler::append(const QString &_data, const QString &_file)
{
    QFile file(_file);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream stream(&file);
        stream << _data << "\n";

        file.close();
    }
    else
        qDebug() << "FileHandler::append failed" << _file;
}

QVector<double> FileHandler::parseCSV(const QString &_line)
{
    QVector<double> values;
    foreach(QString value, _line.split(","))
        values << value.toDouble();

    return values;
}


/*************************************
 *           PRIVATE METHODS         *
 ************************************/

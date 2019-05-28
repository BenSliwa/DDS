#include "filehandler.h"
#include <QFile>
#include <QTextStream>

FileHandler::FileHandler()
{

}

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

    return lines;
}

void FileHandler::append(const QString &_data, const QString &_path)
{
    QFile file(_path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream stream(&file);
        stream << _data << "\n";

        file.close();
    }
}

void FileHandler::write(const QString &_data, const QString &_path)
{
    QFile file(_path);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        stream << _data << "\n";

        file.close();
    }
}

QVector<double> FileHandler::parseCSV(const QString &_line)
{
    QVector<double> values;
    foreach(QString value, _line.split(","))
        values << value.toDouble();

    return values;
}


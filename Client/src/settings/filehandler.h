#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>
#include <QDebug>
#include <QStringList>

class FileHandler
{
public:
    FileHandler();

    static QStringList read(const QString &_file);
    static void append(const QString &_data, const QString &_path);
    static void write(const QString &_data, const QString &_path);
    static QVector<double> parseCSV(const QString &_line);
};

#endif // FILEHANDLER_H

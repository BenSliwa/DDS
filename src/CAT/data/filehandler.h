#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QDebug>

class FileHandler
{
public:
    FileHandler();

    static QStringList read(const QString &_file);
    static void write(const QString &_data, const QString &_file);

    static QVector<double> parseCSV(const QString &_line);
};

#endif // FILEHANDLER_H

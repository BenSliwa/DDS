#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <QDebug>

class DataParser
{
public:
    DataParser();

    void parseFile(const QString &_path);
    QStringList parseHeader(const QString &_line);

private:
    QStringList m_header;
};

#endif // DATAPARSER_H

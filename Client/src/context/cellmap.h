#ifndef CELLMAP_H
#define CELLMAP_H

#include <QDebug>

class CellMap
{
public:
    CellMap();

    void init(const QString &_file);

    bool hasEntry(int _id);
    double getEntry(int _id);

private:
    QMap<int, double> m_frequencies;
};

#endif // CELLMAP_H

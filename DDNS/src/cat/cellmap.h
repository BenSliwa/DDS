#ifndef CELLMAP_H
#define CELLMAP_H

#include <QObject>
#include <QDebug>

class CellMap : public QObject
{
    Q_OBJECT
public:
    CellMap(QObject *_parent = 0);

    void init(const QString &_mno);

    bool hasEntry(int _id);
    double getEntry(int _id);

private:
    QMap<int, double> m_frequencies;
};

#endif // CELLMAP_H

#ifndef CONNECTIVITYMAP_H
#define CONNECTIVITYMAP_H

#include <QObject>
#include <QDebug>
#include <QVector2D>

struct ConnectivityMapEntry
{
    bool isValid;
    float rsrp_dBm;
    float rsrq_dB;
    float sinr_dB;
    float cqi;
    float ss;
    float ta;
    float velocity_mps;
    int ci;
    float f_MHz;
};

class ConnectivityMap : public QObject
{
    Q_OBJECT
public:
    ConnectivityMap(QObject *_parent = 0);

    void init();
    ConnectivityMapEntry getEntry(double _lat, double _lon);

private:
    QVector2D m_origin;
    QVector2D m_inc;
    QMap<QString, ConnectivityMapEntry> m_entries;

};

#endif // CONNECTIVITYMAP_H

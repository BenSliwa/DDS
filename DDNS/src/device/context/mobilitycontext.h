#ifndef MOBILITYCONTEXT_H
#define MOBILITYCONTEXT_H

#include <QObject>
#include <QDebug>
#include <QVariantMap>
#include <QGeoPositionInfoSource>

class MobilityContext : public QObject
{
    Q_OBJECT
public:
    MobilityContext(QObject *_parent = 0);

    static MobilityContext* getInstance();

    QVariantMap getData();

private slots:
    void onPositionUpdated(QGeoPositionInfo _info);
    void onError(QGeoPositionInfoSource::Error _error);

private:
    QVariantMap m_data;
    QGeoPositionInfoSource *m_source;
};

#endif // MOBILITYCONTEXT_H

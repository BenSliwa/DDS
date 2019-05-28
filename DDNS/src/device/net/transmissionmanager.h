#ifndef TRANSMISSIONMANAGER_H
#define TRANSMISSIONMANAGER_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include "cat/cat.h"
#include "uplink.h"
#include "downlink.h"
#include "device/context/contextmanager.h"

class TransmissionManager : public QObject
{
    Q_OBJECT
public:
    TransmissionManager(QObject *_parent = 0);

    void start();
    void stop();
    void addTransmissionScheme(CAT *_cat);



private:
    void updateTransmissionSchemes(const Context &_context);


public slots:
    void onTimeout();
    void onTransmitted(double _prediction, double _buffer_mb, double _deltaT_s, const QString &_context);
    void onSuccess(double _throughput_mbits, double _payload_mb, double _rtt_ms, const QString &_direction);

signals:
    void transmitted(double _throughput_mbits, const QString &_dir);

private:
    QTimer m_timer;
    QVector<CAT*> m_schemes;
    ContextManager *p_context;

    QString m_address;
    int m_port;

    QMap<Transmission*, QString> m_transmissions;
    int m_uplink;
    int m_downlink;
};

#endif // TRANSMISSIONMANAGER_H

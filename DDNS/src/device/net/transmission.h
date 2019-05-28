#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDataStream>

enum states{RTT, THROUGHPUT};
enum types{UPLINK, DOWNLINK};

class Transmission : public QObject
{
    Q_OBJECT
public:
    Transmission(const QString &_id, int _type, QObject *_parent = 0);
    void start(int _bytes, const QString &_address, int _port);

    static qint64 getTimestamp();
    QByteArray readBytes(const QString &_file);

    QString getDummyDataPath();

    //
    QString getId();
    int getType();

protected:
    virtual void transfer() = 0;
    virtual void handleReply(const QByteArray &_data, qint64 _timestamp_ms) = 0;

private slots:
    void onReadyRead();
    void onConnected();

signals:
    void success(double _throughput_mbits, double _payload_mb, double _rtt_ms, const QString &_direction);


protected:
    QTcpSocket m_socket;
    QString m_id;
    int m_type;
    int m_state;
    int m_bytes;

    double m_rtt_ms;
    double m_tx_ms;

};

#endif // TRANSMISSION_H

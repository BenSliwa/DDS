#ifndef DATATRANSFER_H
#define DATATRANSFER_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QMap>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>
#include <QDataStream>
#include "settings/filehandler.h"
#include <QTextStream>
#include "context/prediction/TXPOWER_forest.h"

class DataTransfer : public QObject
{
    Q_OBJECT
public:
    DataTransfer(const QString &_id, const QVariantMap &_context, int _type, int _index = -1, QObject *_parent = 0);
    static qint64 getTimestamp();

protected:
    int getRandomInt(int _min, int _max);
    QByteArray readBytes(const QString &_file);
    void handleTransmission(double _throughput_mbps, const QString &_dir);

    void writeLog(double _throughput_mbits, double _payload_mb, double _delay_ms);


    virtual void transfer() = 0;
    virtual void handleReply(const QByteArray &_data, qint64 _timestamp_ms) = 0;

private slots:
    void onReadyRead();
    void onConnected();
    void onDisconnected();
    void onBytesWritten();

protected:
    QTcpSocket m_socket;
    QString m_id;
    QVariantMap m_context;
    int m_type;
    int m_state;
    int m_index;


    double m_rtt_ms;
    double m_tx_ms;

    QVector<int> m_payloads;
    enum states{RTT, THROUGHPUT};
    enum types{UPLINK, DOWNLINK};

};

#endif // DATATRANSFER_H

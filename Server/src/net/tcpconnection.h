#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <QDebug>
#include <QTcpSocket>
#include <QDateTime>
#include <QVariantMap>

class TcpConnection : public QObject
{
    Q_OBJECT
public:
    TcpConnection(QTcpSocket *_socket, QObject *_parent = 0);

private:
    void handleMessage(const QVariantMap &_message);
    void sendMessage(const QVariantMap &_message);
    qint64 getTimestamp();
    int getRandomInt(int _min, int _max);
    QByteArray readBytes(const QString &_file);

private slots:
    void onReadyRead();
    void onDisconnected();
    void onBytesWritten();

private:
    QTcpSocket *p_socket;
    QByteArray m_buffer;
    qint64 m_transmissionStart;


    enum types{UPLINK, DOWNLINK};
    int m_type;
    qint32 m_expectedBytes;

};

#endif // TCPCONNECTION_H

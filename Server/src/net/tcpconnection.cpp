#include "tcpconnection.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFile>
#include <QDebug>
#include <QDataStream>

TcpConnection::TcpConnection(QTcpSocket *_socket, QObject *_parent) :
    QObject(_parent),
    p_socket(_socket),
    m_type(UPLINK),
    m_expectedBytes(0)
{
    connect(p_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(p_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(p_socket, SIGNAL(bytesWritten(qint64)), this, SLOT(onBytesWritten()));
    p_socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
}

void TcpConnection::handleMessage(const QVariantMap &_message)
{
    qDebug() << "TcpConnection::handleMessage" << _message["transmission"];


    if(m_type==UPLINK)
    {
        // send an ACK containing the measured transmission efficiency parameters
        QVariantMap message;
        message["type"] = "ACK";
        message["transmission"] = _message["transmission"];
        message["context"] = _message["context"];

        sendMessage(message);
    }
    else if(m_type==DOWNLINK)
    {
    }
}

void TcpConnection::sendMessage(const QVariantMap &_message)
{
    qDebug() << "TcpConnection::sendMessage";

    QJsonObject obj = QJsonObject::fromVariantMap(_message);
    QJsonDocument doc;
    doc.setObject(obj);

    p_socket->write(doc.toJson());
}

qint64 TcpConnection::getTimestamp()
{
    return QDateTime::currentMSecsSinceEpoch();
}

int TcpConnection::getRandomInt(int _min, int _max)
{
    int number = qrand() % ((_max + 1) - _min) + _min;
    return number;
}

QByteArray TcpConnection::readBytes(const QString &_file)
{
    QByteArray data;
    QFile file(_file);
    if (file.open(QIODevice::ReadOnly))
        data = file.readAll();

    return data;
}

void TcpConnection::onReadyRead()
{
    qint64 timestamp_ms = getTimestamp();

    //
    QByteArray data;
    while(p_socket->bytesAvailable())
        data.append(p_socket->readAll());

    // obtain the connection type and echo the timestamp
    if(data.size()<20)
    {
        QStringList fields = QString::fromUtf8(data).split(",");

        qDebug() << fields;

        if(fields.size()==1 && m_type==DOWNLINK)
        {
            qint64 requestedBytes = fields.at(0).toLongLong();
            QVector<int>  payloads;
            payloads << 104858 << 524288 << 1048576 << 2097152 << 3145728 << 4194304;
            payloads << 5242880 << 6291456 << 7340032 << 8388608 << 9437184 << 10485760;

            QByteArray data;
            if(payloads.contains(requestedBytes))
                data = readBytes("sdcard/" + QString::number(requestedBytes));
            else
            {
                int chunks = requestedBytes / 52429;
                QByteArray chunk = readBytes("/sdcard/" + QString::number(52429));
                for(int i=0; i<chunks; i++)
                    data.append(chunk);

                QByteArray header;
                QDataStream stream(&header, QIODevice::WriteOnly);
                stream << (qint32)data.size();

                data[0] = header[0];
                data[1] = header[1];
                data[2] = header[2];
                data[3] = header[3];

            }



            m_transmissionStart = getTimestamp();
            p_socket->write(data);
        }
        else if(fields.size()==2)
        {
            m_type = fields.at(1).toInt();
            p_socket->write(data);
            p_socket->flush();
        }

        return;
    }

    if(m_buffer.size()==0)
    {
        m_transmissionStart = timestamp_ms;

        // parse the header
        QByteArray header = data.mid(0, 4);
        QDataStream stream(header);
        stream >> m_expectedBytes;

        if(m_type==UPLINK)
        {
            qDebug() << "expecting uplink of" << m_expectedBytes;
        }
        else
        {
            qDebug() << "requested downlink of" << m_expectedBytes;
        }
    }
    m_buffer.push_back(data);



    if(m_buffer.size()==m_expectedBytes)
    {
        QVariantMap message;

        // handle the received message
        double duration_s = (timestamp_ms - m_transmissionStart)/1000.0;
        double payload_mb = (double)m_buffer.size() / (1024 * 1024);
        double throughput_mbits = payload_mb * 8.0 / duration_s;

        QVariantMap transmission;
        transmission["duration"] = duration_s;
        transmission["payload"] = payload_mb;
        transmission["throughput"] = throughput_mbits;
        message["transmission"] = transmission;

        //
        m_buffer.clear();
        handleMessage(message);
    }
    else if(m_buffer.size()>m_expectedBytes)
    {
        // something went wrong
    }
}

void TcpConnection::onDisconnected()
{
    qDebug() << "TcpConnection::onDisconnected";

    this->deleteLater();
}

void TcpConnection::onBytesWritten()
{
    qDebug() << "TcpConnection::onBytesWritten" << getTimestamp()-m_transmissionStart;
}

#include "uplink.h"

Uplink::Uplink(const QString &_id, QObject *_parent) :
    Transmission(_id, UPLINK, _parent)
{

}


/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void Uplink::transfer()
{
    int chunks = m_bytes / 52429;

    QByteArray data;
    QByteArray chunk = readBytes(getDummyDataPath() + QString::number(52429));
    for(int i=0; i<chunks; i++)
        data.append(chunk);

    QByteArray header;
    QDataStream stream(&header, QIODevice::WriteOnly);
    stream << (qint32)data.size();

    data[0] = header[0];
    data[1] = header[1];
    data[2] = header[2];
    data[3] = header[3];

    m_tx_ms = getTimestamp();

    m_socket.write(data);
    m_socket.flush();
}

void Uplink::handleReply(const QByteArray &_data, qint64 _timestamp_ms)
{
    QJsonDocument doc = QJsonDocument::fromJson(_data);
    if(!doc.isNull())
    {
        QVariantMap message = doc.object().toVariantMap();
        QVariantMap transmission = message["transmission"].toMap();
        transmission["rtt"] = m_rtt_ms;
        message["transmission"] = transmission;

        double throughput_mbits = transmission["throughput"].toDouble();
        double payload_mb = transmission["payload"].toDouble();
        emit success(throughput_mbits, payload_mb, m_rtt_ms, "ul");

        qDebug() << "Uplink::handleReply"  << throughput_mbits << payload_mb << m_rtt_ms;
    }

    m_socket.disconnectFromHost();
    this->deleteLater();
}


/*************************************
 *           PRIVATE METHODS         *
 ************************************/


/*************************************
 *            PRIVATE SLOTS          *
 ************************************/

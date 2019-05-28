#include "uplink.h"
#include "uimanager.h"

Uplink::Uplink(const QString &_id, const QVariantMap &_context, int _index, QObject *_parent) :
    DataTransfer(_id, _context, UPLINK, _index, _parent)
{


}

void Uplink::transfer()
{
    // generate the payload
    int index = getRandomInt(0, m_payloads.size()-1);
    if(m_index>11)
    {
        int chunks = m_index / 52429;

        QByteArray data;
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

        m_tx_ms = getTimestamp();

        m_socket.write(data);
        m_socket.flush();
    }
    else
    {
        int payload = m_payloads.at(index);

        QByteArray data = readBytes("/sdcard/" + QString::number(payload));

        m_tx_ms = getTimestamp();

        m_socket.write(data);
        m_socket.flush();
    }
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

        double throughput = transmission["throughput"].toDouble();
        handleTransmission(throughput, "UL");

        qDebug() << "uplink" << m_rtt_ms << throughput << message["transmission"];
        writeLog(throughput, transmission["payload"].toDouble(), m_rtt_ms);
    }

    m_socket.disconnectFromHost();
    this->deleteLater();
}


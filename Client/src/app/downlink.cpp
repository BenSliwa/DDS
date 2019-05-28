#include "downlink.h"
#include "uimanager.h"

Downlink::Downlink(const QString &_id, const QVariantMap &_context, int _index, QObject *_parent) :
    DataTransfer(_id, _context, DOWNLINK, _index, _parent),
    m_buffer(0)
{

}

void Downlink::transfer()
{
    int index = getRandomInt(0, m_payloads.size()-1);
    if(m_index>11)
    {
        m_expectedBytes = m_index;
    }
    else
    {
        if(m_index>-1)
            index = m_index;

        m_expectedBytes = m_payloads.at(index);
    }

    m_tx_ms = getTimestamp();
    m_socket.write(QString::number(m_expectedBytes).toUtf8());

    qDebug() << "Downlink::transfer" << m_expectedBytes;
}

void Downlink::handleReply(const QByteArray &_data, qint64 _timestamp_ms)
{


    //
    if(m_buffer==0)
        m_transmissionStart = _timestamp_ms;
    m_buffer += _data.size();


     qDebug() << "Downlink::handleReply" << m_buffer << m_expectedBytes;

    //
    if(m_buffer==m_expectedBytes)
    {
        // handle the received message
        QVariantMap message;
        double duration_s = (_timestamp_ms - m_transmissionStart)/1000.0;
        double payload_mb = (double)m_buffer / (1024.0 * 1024.0);
        double throughput_mbits = payload_mb * 8.0 / duration_s;

        QVariantMap downlink;
        downlink["duration"] = duration_s;
        downlink["payload"] = payload_mb;
        downlink["throughput"] = throughput_mbits;
        message["downlink"] = downlink;

        QVariantMap transmission = message["transmission"].toMap();
        transmission["rtt"] = m_rtt_ms;
        message["transmission"] = transmission;


        //
        m_buffer = 0;


        // handle message

        handleTransmission(throughput_mbits, "DL");

        qDebug() << "downlink" << m_rtt_ms << message["downlink"];
        writeLog(throughput_mbits, payload_mb, m_rtt_ms);


        m_socket.disconnectFromHost();
        this->deleteLater();
    }
    else if(m_buffer>m_expectedBytes)
    {
        // something went wrong
    }

}

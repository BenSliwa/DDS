#include "downlink.h"

Downlink::Downlink(const QString &_id, QObject *_parent) :
    Transmission(_id, DOWNLINK, _parent),
    m_buffer(0)
{

}


/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void Downlink::transfer()
{
    m_expectedBytes = m_bytes;
    m_tx_ms = getTimestamp();
    m_socket.write(QString::number(m_expectedBytes).toUtf8());
}

void Downlink::handleReply(const QByteArray &_data, qint64 _timestamp_ms)
{
    if(m_buffer==0)
        m_transmissionStart = _timestamp_ms;
    m_buffer += _data.size();

    //
    if(m_buffer==m_expectedBytes)
    {
        double duration_s = (_timestamp_ms - m_transmissionStart)/1000.0;
        double payload_mb = (double)m_buffer / (1024.0 * 1024.0);
        double throughput_mbits = payload_mb * 8.0 / duration_s;
        m_buffer = 0;

        qDebug() << "Downlink::handleReply" << throughput_mbits << payload_mb << m_rtt_ms;

        emit success(throughput_mbits, payload_mb, m_rtt_ms, "dl");

        m_socket.disconnectFromHost();
        this->deleteLater();
    }
    else if(m_buffer>m_expectedBytes)
    {
        // something went wrong
    }
}

/*************************************
 *           PRIVATE METHODS         *
 ************************************/


/*************************************
 *            PRIVATE SLOTS          *
 ************************************/

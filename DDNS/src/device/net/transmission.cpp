#include "transmission.h"
#include <QDateTime>
#include <QFile>

Transmission::Transmission(const QString &_id, int _type, QObject *_parent) :
    QObject(_parent),
    m_id(_id),
    m_type(_type),
    m_state(RTT)
{
    //qsrand(getTimestamp());
    qsrand(0);
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void Transmission::start(int _bytes, const QString &_address, int _port)
{
    m_bytes = _bytes;

    connect(&m_socket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(&m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));

    m_socket.connectToHost(_address, _port);
}

qint64 Transmission::getTimestamp()
{
    return QDateTime::currentMSecsSinceEpoch();
}

QByteArray Transmission::readBytes(const QString &_file)
{
    QByteArray data;
    QFile file(_file);
    if (file.open(QIODevice::ReadOnly))
        data = file.readAll();
    else
        qDebug() << "Transmission::readBytes failed to open" << _file;

    return data;
}

QString Transmission::getDummyDataPath()
{
    #ifdef Q_OS_ANDROID
        return "/sdcard/";
    #else
        return "";
    #endif
}

QString Transmission::getId()
{
    return m_id;
}

int Transmission::getType()
{
    return m_type;
}


/*************************************
 *           PRIVATE METHODS         *
 ************************************/


/*************************************
 *            PRIVATE SLOTS          *
 ************************************/

void Transmission::onReadyRead()
{
    qint64 timestamp_ms = getTimestamp();
    QByteArray data;
    while(m_socket.bytesAvailable())
        data.append(m_socket.readAll());

    if(m_state==RTT)
    {
        QStringList fields = QString::fromUtf8(data).split(",");
        if(fields.size()==2)
        {
            qint64 tx_ms = fields.at(0).toLongLong();
            m_rtt_ms = timestamp_ms - tx_ms;

            m_state = THROUGHPUT;
            transfer();
        }
    }
    else if(m_state==THROUGHPUT)
    {
        handleReply(data, timestamp_ms);
    }
}

void Transmission::onConnected()
{
    m_socket.setSocketOption(QAbstractSocket::LowDelayOption, 1);

    // send a delay probing message with a timestamp and the transfer type
    QString message;
    QTextStream stream(&message);

    stream << getTimestamp() << "," << m_type;
    m_socket.write(message.toUtf8());
}

#include "datatransfer.h"
#include "uimanager.h"

DataTransfer::DataTransfer(const QString &_id, const QVariantMap &_context, int _type, int _index, QObject *_parent) :
    QObject(_parent),
    m_id(_id),
    m_context(_context),
    m_type(_type),
    m_index(_index),
    m_state(RTT)
{
    qsrand(getTimestamp());
    m_payloads << 104858 << 524288 << 1048576 << 2097152 << 3145728 << 4194304;
    m_payloads << 5242880 << 6291456 << 7340032 << 8388608 << 9437184 << 10485760;

    connect(&m_socket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(&m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(&m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(&m_socket, SIGNAL(bytesWritten(qint64)), this, SLOT(onBytesWritten()));

    QString address = "";
    int port = -1;

    m_socket.connectToHost(address, port);
}

qint64 DataTransfer::getTimestamp()
{
    return QDateTime::currentMSecsSinceEpoch();
}

int DataTransfer::getRandomInt(int _min, int _max)
{
    int number = qrand() % ((_max + 1) - _min) + _min;
    return number;
}

QByteArray DataTransfer::readBytes(const QString &_file)
{
    QByteArray data;
    QFile file(_file);
    if (file.open(QIODevice::ReadOnly))
        data = file.readAll();
    else
        qDebug() << "DataTransfer::readBytes failed to open" << _file;

    return data;
}

void DataTransfer::handleTransmission(double _throughput_mbps, const QString &_dir)
{
    UiManager *ui = UiManager::getInstance();
    ui->handleTransmission(_throughput_mbps, _dir);
}

void DataTransfer::writeLog(double _throughput_mbits, double _payload_mb, double _delay_ms)
{
    UiManager *ui = UiManager::getInstance();
    QString path = ui->getDataPath() + ui->getSession();
    if(m_type==UPLINK)
        path = path + "_ul.txt";
    else
        path = path + "_dl.txt";

    ContextManager *cm = ContextManager::getInstance();
    Context context = cm->createContextEntry(m_context);
    QString contextEntry = cm->serializeContextEntry(context);

    QString log;
    QTextStream stream(&log);
    stream << contextEntry << "," << m_id << "," << _payload_mb << "," << _throughput_mbits << "," << _delay_ms;

    if(m_type==UPLINK)
    {
        // determine the transmission power
        float rsrp = context.data["rsrp"].toDouble();
        float rsrq = context.data["rsrq"].toDouble();
        float rssnr = context.data["rssnr"].toDouble();
        float velocity_kmh = context.data["velocity"].toDouble() * 3.6;
        QVector<float> features = QVector<float>() << rsrp << rsrq << rssnr << velocity_kmh << _payload_mb;
        float txPower_dBm = txpower_predict(features.constData());

        stream << "," << txPower_dBm;
    }

    FileHandler::append(log, path);
}

void DataTransfer::onReadyRead()
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

void DataTransfer::onConnected()
{
    m_socket.setSocketOption(QAbstractSocket::LowDelayOption, 1);

    // send a delay probing message with a timestamp and the transfer type
    QString message;
    QTextStream stream(&message);

    stream << getTimestamp() << "," << m_type;
    m_socket.write(message.toUtf8());
}

void DataTransfer::onDisconnected()
{

}

void DataTransfer::onBytesWritten()
{
    //qDebug() << "DataTransfer::onBytesWritten" << getTimestamp()-m_tx_ms;
}

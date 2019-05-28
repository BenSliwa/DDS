#ifndef DOWNLINK_H
#define DOWNLINK_H

#include "transmission.h"

class Downlink : public Transmission
{
    Q_OBJECT
public:
    Downlink(const QString &_id, QObject *_parent = 0);

protected:
    void transfer();
    void handleReply(const QByteArray &_data, qint64 _timestamp_ms);

private:
    qint64 m_buffer;
    qint64 m_transmissionStart;
    qint64 m_expectedBytes;
};

#endif // DOWNLINK_H

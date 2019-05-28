#ifndef DOWNLINK_H
#define DOWNLINK_H

#include "datatransfer.h"

class Downlink : public DataTransfer
{
    Q_OBJECT
public:
    Downlink(const QString &_id, const QVariantMap &_context, int _index = -1, QObject *_parent = 0);

    virtual void transfer();
    virtual void handleReply(const QByteArray &_data, qint64 _timestamp_ms);

private:
    qint64 m_buffer;
    qint64 m_transmissionStart;
    qint64 m_expectedBytes;

};

#endif // DOWNLINK_H

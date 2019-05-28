#ifndef UPLINK_H
#define UPLINK_H

#include "transmission.h"

class Uplink : public Transmission
{
    Q_OBJECT
public:
    Uplink(const QString &_id, QObject *_parent = 0);

protected:
    void transfer();
    void handleReply(const QByteArray &_data, qint64 _timestamp_ms);
};

#endif // UPLINK_H

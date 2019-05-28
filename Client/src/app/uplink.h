#ifndef UPLINK_H
#define UPLINK_H

#include "datatransfer.h"

class Uplink : public DataTransfer
{
    Q_OBJECT
public:
    Uplink(const QString &_id, const QVariantMap &_context, int _index = -1, QObject *_parent = 0);

    virtual void transfer();
    virtual void handleReply(const QByteArray &_data, qint64 _timestamp_ms);



};

#endif // UPLINK_H

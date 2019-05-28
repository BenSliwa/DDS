#ifndef CONTEXTMANAGER_H
#define CONTEXTMANAGER_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include "device/context/context.h"
#include "device/context/mobilitycontext.h"
#include "device/context/networkcontext.h"

class ContextManager : public QObject
{
    Q_OBJECT
public:
    ContextManager(QObject *_parent = 0);

    static ContextManager* getInstance();

    void start();
    void stop();
    qint64 getTimestamp();
    QString getSession();

    QString serializeContextEntry(const Context &_context);
    void logContext();
    void logCells();

    Context createContextEntry(const QVariantMap &_data);
    Context getContext();

private slots:
    void onTimeout();

private:
    QTimer m_timer;

    NetworkContext *p_network;
    MobilityContext *p_mobility;

    Context m_context;
    int m_counter;
    bool m_active;
        qint64 m_startTime_ms;
};

#endif // CONTEXTMANAGER_H

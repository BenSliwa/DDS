#ifndef CONTEXTMANAGER_H
#define CONTEXTMANAGER_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include "mobilitycontext.h"
#include "networkcontext.h"
#include "context.h"

class ContextManager : public QObject
{
    Q_OBJECT
public:
    ContextManager(QObject *_parent = 0);
    static ContextManager* getInstance();

    void start();
    void stop();

    Context createContextEntry(const QVariantMap &_data);
    Context getContext();
    QString getCarrier();
    QString serializeContextEntry(const Context &_context);

private:
    void logContext();
    void logCells();

private slots:
    void onUpdated();

signals:
    void updated(const QVariantMap &_data);

private:
    QTimer m_timer;
    MobilityContext m_mobilityContext;
    NetworkContext m_networkContext;
    int m_counter;

    bool m_active;
    Context m_context;
};

#endif // CONTEXTMANAGER_H

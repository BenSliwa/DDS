#ifndef NETWORKCONTEXT_H
#define NETWORKCONTEXT_H

#include <QObject>
#include <QTimer>
#include <QVariantMap>

class NetworkContext : public QObject
{
    Q_OBJECT
public:
    NetworkContext(QObject *_parent = 0);

    static NetworkContext* getInstance();

    QVariantMap getData();
    QVariantList getCells();

    Q_INVOKABLE QString getCarrier();

private slots:
    void onTimeout();

private:
    QTimer m_timer;
    QVariantMap m_data;

    QString m_carrier;

};

#endif // NETWORKCONTEXT_H

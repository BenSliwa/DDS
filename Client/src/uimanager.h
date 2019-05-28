#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QObject>
#include <QDebug>
#include <QtQuick>
#include <QTimer>
#include "app/dataupload.h"
#include "context/contextmanager.h"

class UiManager : public QObject
{
    Q_OBJECT
public:
    UiManager(QObject *_parent = 0);
    static UiManager* getInstance();

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

    double getTimestamp();
    int getRawTimestamp();

    QString getSession();
    static QString getDataPath();

    void handleTransmission(double _throughput_mbps, const QString &_dir);

    static QString getCarrier();

signals:
    void resetUi();
    void transmitted(double _throughput_mbps);
    void transmitted(double _throughput_mbps, const QString &_dir);


private:
    QQuickView m_qml;
    ContextManager *m_contextManager;
    DataUpload *m_app;

    double m_timestamp_s;
    QString m_session;

};

#endif // UIMANAGER_H

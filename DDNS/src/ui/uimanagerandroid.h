#ifndef UIMANAGERANDROID_H
#define UIMANAGERANDROID_H

#include <QDebug>
#include <QtQuick/QQuickView>
#include "device/net/transmissionmanager.h"

class UiManagerAndroid : public QObject
{
    Q_OBJECT
public:
    UiManagerAndroid(QObject *_parent = 0);

    static UiManagerAndroid* getInstance();

    //
    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();


signals:
    void started();
    void stopped();
    void transmitted(double _throughput_mbits, const QString &_dir);

private:
    QQuickView m_qml;
    TransmissionManager *m_transmissionManager;
};

#endif // UIMANAGERANDROID_H

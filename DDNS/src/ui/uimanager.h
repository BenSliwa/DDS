#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QObject>
#include <QDebug>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlEngine>
#include <QTimer>
#include "cat/mobilityprediction.h"


class UiManager : public QObject
{
    Q_OBJECT
public:
    UiManager(QObject *_parent = 0);

    void loadScenario(const QStringList &_files);

    Q_INVOKABLE void handleKeyPress(int _key);



private slots:
    void onTimeout();

signals:
    void positionUpdated(double _lat, double _lon);
    void predictionUpdated(double _lat, double _lon);


private:
    QQuickView m_qml;
    QTimer m_timer;

    QStringList m_data;
    MobilityPrediction m_mobilityPrediction;
};

#endif // UIMANAGER_H

#include "uimanager.h"
#include <QDateTime>
#include "context/prediction/m5t.h"
#include "context/prediction/TXPOWER_forest.h"
#include "app/cattransfer.h"

UiManager::UiManager(QObject *_parent) :
    QObject(_parent),
    m_contextManager(0),
    m_app(0)
{
    m_timestamp_s = 0;
    m_timestamp_s = getTimestamp();

    m_contextManager = ContextManager::getInstance();

    m_qml.rootContext()->setContextProperty("UiManager", this);
    m_qml.rootContext()->setContextProperty("ContextManager", m_contextManager);

    m_qml.setResizeMode(QQuickView::SizeRootObjectToView);
    m_qml.setSource(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    m_qml.show();
}

void UiManager::start()
{
    emit resetUi();

    // generate new file ids
    m_session = QString::number(getRawTimestamp(), 'g', 10) + "_" + m_contextManager->getCarrier();

    qDebug() << "UiManager::start" << m_session;

    //
    m_timestamp_s = 0;
    m_timestamp_s = getTimestamp();

    //m_app = new DataUpload(this);
    m_app = new CATTransfer(this);
    connect(m_app, SIGNAL(transmitted(double)), this, SIGNAL(transmitted(double)));

    qDebug() << "UiManager::start" << getSession() << getDataPath();

    m_contextManager->start();
}

void UiManager::stop()
{
    m_app->deleteLater();
    m_contextManager->stop();
}

UiManager* UiManager::getInstance()
{
    static UiManager instance;
    return &instance;
}

double UiManager::getTimestamp()
{
    return QDateTime::currentMSecsSinceEpoch()/1000.0 - m_timestamp_s;
}

int UiManager::getRawTimestamp()
{
    return QDateTime::currentMSecsSinceEpoch()/1000.0;
}

QString UiManager::getSession()
{
    return m_session;
}

QString UiManager::getDataPath()
{
    #ifdef Q_OS_ANDROID
        return "/sdcard/CAT/";
    #else
        return "/";
    #endif
}

void UiManager::handleTransmission(double _throughput_mbps, const QString &_dir)
{
    emit transmitted(_throughput_mbps, _dir);
}

QString UiManager::getCarrier()
{
    QString carrier = "";
    QString folder = "/sdcard/";
    QFile tmobile(folder + "tmobile.txt");
    QFile o2(folder + "o2.txt");
    QFile vodafone(folder + "vodafone.txt");
    if(tmobile.exists())
        carrier = "tmobile";
    else if(o2.exists())
        carrier = "o2";
    else if(vodafone.exists())
        carrier = "vodafone";

    return carrier;
}


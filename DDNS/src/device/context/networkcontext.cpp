#include "networkcontext.h"
#ifdef Q_OS_ANDROID
    #include <QtAndroidExtras/QAndroidJniObject>
#endif
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QFile>

NetworkContext::NetworkContext(QObject *_parent) :
    QObject(_parent),
    m_carrier("")
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    m_timer.start(100);

    QString folder = "/sdcard/";
    QFile tmobile(folder + "tmobile.txt");
    QFile o2(folder + "o2.txt");
    QFile vodafone(folder + "vodafone.txt");

    if(tmobile.exists())
        m_carrier = "tmobile";
    else if(o2.exists())
        m_carrier = "o2";
    else if(vodafone.exists())
        m_carrier = "vodafone";
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

NetworkContext* NetworkContext::getInstance()
{
    static NetworkContext instance;
    return &instance;
}

QVariantMap NetworkContext::getData()
{
    return m_data["connected"].toMap();
}

QVariantList NetworkContext::getCells()
{
    return m_data["cells"].toList();
}

/*************************************
 *           PRIVATE METHODS         *
 ************************************/


/*************************************
 *            PRIVATE SLOTS          *
 ************************************/

void NetworkContext::onTimeout()
{
    #ifdef Q_OS_ANDROID
        QAndroidJniObject str = QAndroidJniObject::callStaticObjectMethod<jstring>("org/qtproject/example/notification/NetworkInfo", "readCellularInfo");
        QJsonDocument doc = QJsonDocument::fromJson(str.toString().toUtf8());
        m_data = doc.object().toVariantMap();



        /*
        //
        QVariantMap cell = getData();
        if(m_carrier=="" && cell.contains("mnc"))
        {
            int mnc = cell["mnc"].toInt();
            switch(mnc)
            {
                case 1: m_carrier="tmobile"; break;
                case 2: m_carrier="vodafone"; break;
                case 3: m_carrier="o2"; break;
            }
        }*/
    #else
        m_data.clear();

        QVariantMap raw;
        raw["isRegistered"] = 1;
        raw["rsrp"] = -80;
        raw["rssnr"] = 10;
        raw["rsrq"] = -4;
        raw["cqi"] = 10;
        m_data["conntected"] = raw;
    #endif


}

QString NetworkContext::getCarrier()
{
    return m_carrier;
}

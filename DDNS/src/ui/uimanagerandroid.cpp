#include "uimanagerandroid.h"
#include <QQmlContext>
#include "device/context/mobilitycontext.h"
#include "device/context/networkcontext.h"
#include "device/context/contextmanager.h"


UiManagerAndroid::UiManagerAndroid(QObject *_parent) :
    QObject(_parent)
{
    QQmlContext *ctx = m_qml.rootContext();
    ctx->setContextProperty("UiManager", this);

    MobilityContext *mobility = MobilityContext::getInstance();
    NetworkContext *network = NetworkContext::getInstance();
    ContextManager *context = ContextManager::getInstance();
    ctx->setContextProperty("NetworkContext", network);

    m_qml.setTitle(QStringLiteral("DDS"));
    m_qml.setSource(QUrl("qrc:/ui/qml/main_android.qml"));
    m_qml.setResizeMode(QQuickView::SizeRootObjectToView);
    m_qml.show();
}

UiManagerAndroid* UiManagerAndroid::getInstance()
{
    static UiManagerAndroid instance;
    return &instance;
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void UiManagerAndroid::start()
{
    qDebug() << "UiManagerAndroid::start";

    m_transmissionManager = new TransmissionManager(this);

    Policy policyUL{POLICY_TYPE::ML_pCAT, 0, 30, 6, 30, 2};
    Policy policyDL{POLICY_TYPE::ML_pCAT, 0, 30, 6, 30, 2};

    QString mno = NetworkContext::getInstance()->getCarrier();
    CAT *ul = new CAT(policyUL, mno, "ul", "key", m_transmissionManager);
    CAT *dl = new CAT(policyDL, mno, "dl", "key", m_transmissionManager);


    QVariantMap mobility = MobilityContext::getInstance()->getData();
    QVector2D position(mobility["latitude"].toDouble(), mobility["longitude"].toDouble());
    QVector2D highway(51.45005366, 7.48829167);
    QVector2D suburban(51.49040466, 7.41248870);

    double highwayDistance_m = MobilityPrediction::computeDistance(position, highway);
    double suburbanDistance_m = MobilityPrediction::computeDistance(position, suburban);

    QString scenario = "highway";
    if(suburbanDistance_m < highwayDistance_m)
        scenario = "suburban";
    ul->clear(scenario);
    dl->clear(scenario);

    m_transmissionManager->addTransmissionScheme(ul);
    m_transmissionManager->addTransmissionScheme(dl);
    m_transmissionManager->start();

    connect(m_transmissionManager, SIGNAL(transmitted(double,QString)), this, SIGNAL(transmitted(double,QString)));

    emit started();
}

void UiManagerAndroid::stop()
{
    qDebug() << "UiManagerAndroid::stop";


    ContextManager::getInstance()->stop();
    m_transmissionManager->stop();

    emit stopped();
}

/*************************************
 *           PRIVATE METHODS         *
 ************************************/

/*************************************
 *            PRIVATE SLOTS          *
 ************************************/

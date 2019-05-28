#include "uimanager.h"
#include <QQmlContext>
#include "data/filehandler.h"


UiManager::UiManager(QObject *_parent) :
    QObject(_parent)
{
    QQmlContext *context = m_qml.rootContext();
    context->setContextProperty("UiManager", this);

    m_qml.setTitle(QStringLiteral("DDS"));
    m_qml.setSource(QUrl("qrc:/ui/qml/main.qml"));
    m_qml.setResizeMode(QQuickView::SizeRootObjectToView);
    m_qml.show();

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

/*************************************
 *            PUBLIC METHODS         *
 ************************************/

void UiManager::loadScenario(const QStringList &_files)
{
    m_data = FileHandler::read(_files.at(0));
    m_timer.start(100);

    m_mobilityPrediction.init("suburban");
}

void UiManager::handleKeyPress(int _key)
{
    if(_key==Qt::Key_Space)
    {
        if(m_timer.isActive())
            m_timer.stop();
        else
            m_timer.start(100);
    }
}


/*************************************
 *           PRIVATE METHODS         *
 ************************************/

/*************************************
 *            PRIVATE SLOTS          *
 ************************************/

void UiManager::onTimeout()
{
    if(m_data.size()>0)
    {
        QVector<double> entry = FileHandler::parseCSV(m_data.at(0));
        if(entry.size()>5)
        {
            double lat = entry.at(3);
            double lon = entry.at(4);
            emit positionUpdated(lat, lon);

            QVector3D prediction = m_mobilityPrediction.predict(lat, lon, 10, entry.at(6));
            emit predictionUpdated(prediction.x(), prediction.y());
        }
        m_data.removeFirst();
    }
    else
        m_timer.stop();
}

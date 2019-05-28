#include "mobilitycontext.h"


MobilityContext::MobilityContext(QObject *_parent) :
    QObject(_parent)
{
    m_data["direction"] = 0;
    m_data["altitude"] = 0;
    m_data["velocity"] = 0;

    #ifdef Q_OS_ANDROID
        m_source = QGeoPositionInfoSource::createDefaultSource(this);
        connect(m_source, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(onPositionUpdated(QGeoPositionInfo)));
        connect(m_source, SIGNAL(error(QGeoPositionInfoSource::Error)), this, SLOT(onError(QGeoPositionInfoSource::Error)));

        m_source->setUpdateInterval(1000);
        m_source->startUpdates();
    #else
        m_data["latitude"] = 50;
        m_data["longitude"] = 7.41;
    #endif
}

QVariantMap MobilityContext::getData()
{
    return m_data;
}

void MobilityContext::onPositionUpdated(QGeoPositionInfo _info)
{
    m_data["latitude"] = _info.coordinate().latitude();
    m_data["longitude"] = _info.coordinate().longitude();
    if(!(_info.coordinate().altitude()!=_info.coordinate().altitude()))
         m_data["altitude"] = _info.coordinate().altitude();

    if(_info.hasAttribute(QGeoPositionInfo::GroundSpeed))
        m_data["velocity"] = _info.attribute(QGeoPositionInfo::GroundSpeed);

    if(_info.hasAttribute(QGeoPositionInfo::Direction))
        m_data["direction"] = _info.attribute(QGeoPositionInfo::Direction);
}

void MobilityContext::onError(QGeoPositionInfoSource::Error _error)
{

}

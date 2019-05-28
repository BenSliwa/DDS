#include "context.h"
#include <QStringList>

Context::Context()
{

}

void Context::add(const QVariantMap &_data)
{
    QStringList keys = _data.keys();
    foreach (QString key, keys)
        data[key] = _data[key];
}

QString Context::getString(const QString &_key, int _precision) const
{
    QString value = "nan";
    if(data.contains(_key))
        value = QString::number(data[_key].toDouble(), 'f', _precision);

    return value;
}

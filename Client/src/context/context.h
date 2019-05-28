#ifndef CONTEXT_H
#define CONTEXT_H

#include <QVariantMap>

class Context
{
public:
    Context();

    void add(const QVariantMap &_data);
    QString getString(const QString &_key, int _precision) const;


public:
    QVariantMap data;
};

#endif // CONTEXT_H

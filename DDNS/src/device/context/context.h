#ifndef CONTEXT_H
#define CONTEXT_H

#include <QVariantMap>

class Context
{
public:
    Context(const QVariantMap &_data = QVariantMap());

    void add(const QVariantMap &_data);
    QString getString(const QString &_key, int _precision) const;

    double get(const QString &_key) const;

public:
    QVariantMap data;
};

#endif // CONTEXT_H

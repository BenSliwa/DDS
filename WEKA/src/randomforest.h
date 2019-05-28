#ifndef RANDOMFOREST_H
#define RANDOMFOREST_H


#include "weka.h"

class RandomForest
{
public:
    RandomForest(const QString &_name);

    void exportForest(const QStringList &_features, const QString &_rawData, const QString &_path);
    QString parseTree(const QString &_tree);

    QString parseIfCondition(const QString &_line, int _deltaDepth);
    QString parseResultStatement(const QString &_line);

    int getDepth(const QString &_line);


private:
    QString m_name;
    int m_leafs;
};

#endif // RANDOMFOREST_H

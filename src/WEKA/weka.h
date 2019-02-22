#ifndef WEKA_H
#define WEKA_H

#include <QProcess>
#include <QDebug>

class WEKA
{
public:
    WEKA();

    void init(const QString &_wekaPath, const QString &_svmWrapperPath, const QString &_svmLibPath);

    QMap<QString, double> runModel(const QString &_file, const QString &_model);
    QString runProcess(const QString &_cmd);

    void analyze(const QString &_folder);
    void confusion(const QStringList &_files, const QString &_path);


    QMap<QString, double> parseResults(const QString &_raw, const QString &_key);
    void analyzeMDI(const QString &_raw, const QString &_file);

    void optimizeANN(const QString &_file);

    static QStringList read(const QString &_file);
    static void write(const QString &_data, const QString &_file);

public:
    QMap<QString, QString> m_models;

    QString m_wekaPath;
    QString m_svmWrapperPath;
    QString m_svmLibPath;
};

#endif // WEKA_H

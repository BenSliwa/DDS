#include "weka.h"
#include <QDir>

WEKA::WEKA()
{
    m_models["ann"] = "weka.classifiers.functions.MultilayerPerceptron -L 0.3 -M 0.2 -N 500 -V 0 -S 0 -E 20 -H a";
    m_models["rf"] = "weka.classifiers.trees.RandomForest -P 100 -I 100 -num-slots 1 -K 0 -M 1.0 -V 0.001 -S 1 -attribute-importance";
    m_models["m5"] = "weka.classifiers.trees.M5P -M 4.0";
    m_models["svm"] = "weka.classifiers.functions.LibSVM -S 4 -K 2 -D 3 -G 0.0 -R 0.0 -N 0.5 -M 40.0 -C 1.0 -E 0.001 -P 0.1 -Z -seed 1";
}


void WEKA::init(const QString &_wekaPath, const QString &_svmWrapperPath, const QString &_svmLibPath)
{
    m_wekaPath = _wekaPath;
    m_svmWrapperPath = _svmWrapperPath;
    m_svmLibPath = _svmLibPath;
}

void WEKA::analyze(const QString &_folder)
{
    QMap<QString, QString>::iterator it;
    for(it=m_models.begin(); it!=m_models.end(); it++)
    {
        QString key = it.key();
        QString model = it.value();

        // analyze all files
        QString data;
        QTextStream stream(&data);
        QDir directory(_folder);
        QStringList files = directory.entryList(QStringList() << "*.csv", QDir::Files);
        foreach(QString file, files)
        {
            QMap<QString, double> result = runModel(_folder + "/" + file, model);
            double corr = result["corr"];
            stream << corr << "," << corr*corr << "," << result["mae"] << "," << result["rmse"] << ","<< result["rae"] << "," << result["rrse"] << "\n";
            qDebug() << result;
        }

        //
        QFile file(_folder + "/results_" + key + ".txt");
        if(file.open(QIODevice::WriteOnly))
        {
            QTextStream stream(&file);
            stream << data;
            file.close();
        }
    }
}

void WEKA::confusion(const QStringList &_files, const QString &_path)
{
    QString data;
    QTextStream resultStream(&data);
    foreach(QString training, _files)
    {
        foreach(QString test, _files)
        {
            QString model = "weka.classifiers.trees.RandomForest -P 100 -I 100 -num-slots 1 -K 0 -M 1.0 -V 0.001 -S 1 -attribute-importance";

            QString exe;
            QTextStream stream(&exe);
            stream << "java -cp " << m_wekaPath << ";" << m_svmWrapperPath << ";" << m_svmLibPath << " ";
            stream << model << " -x 10 -c 1 -t ";
            stream << training;

            if(training!=test)
                stream << " -T " << test;

            QString rawResult = runProcess(exe);
            QMap<QString, double> result;
            if(training!=test)
                result = parseResults(rawResult, "=== Error on test data ===");
            else
                result = parseResults(rawResult, "=== Cross-validation ===");

            double corr = result["corr"];
            resultStream << corr << "," << corr*corr << "," << result["mae"] << "," << result["rmse"] << ","<< result["rae"] << "," << result["rrse"] << "\n";


            qDebug() << result;
        }
    }

    //
    QFile file(_path);
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        stream << data;
        file.close();
    }
}

QMap<QString, double> WEKA::runModel(const QString &_file, const QString &_model)
{
    QString exe;
    QTextStream stream(&exe);
    stream << "java -cp " << m_wekaPath << ";" << m_svmWrapperPath << ";" << m_svmLibPath << " ";
    stream << _model << " -x 10 -c 1 -t ";
    stream << _file;

    QString rawResult = runProcess(exe);

    // feature importance
    if(_model.contains("RandomForest"))
        analyzeMDI(rawResult, _file);

    return parseResults(rawResult, "=== Cross-validation ===");
}

QString WEKA::runProcess(const QString &_cmd)
{
    QProcess pingProcess;
    pingProcess.start(_cmd);
    pingProcess.waitForFinished(-1);

    return pingProcess.readAllStandardOutput();
}

QMap<QString, double> WEKA::parseResults(const QString &_raw, const QString &_key)
{
    // cross validation
    QString crossValidation = _raw.split(_key).last();
    QMap<QString, double> values;
    QStringList lines = crossValidation.split("\n");
    for(int i=0; i<lines.size(); i++)
    {
        QString line = lines.at(i).simplified();
        if(line.contains("Correlation coefficient "))
        {
            line.remove("Correlation coefficient ");
            values["corr"] = line.toDouble();
        }
        else if(line.contains("Mean absolute error "))
        {
            line.remove("Mean absolute error ");
            values["mae"] = line.toDouble();
        }
        else if(line.contains("Root mean squared error "))
        {
            line.remove("Root mean squared error ");
            values["rmse"] = line.toDouble();
        }
        else if(line.contains("Relative absolute error "))
        {
            line.remove("Relative absolute error ");
            line.remove(" %");
            values["rae"] = line.toDouble() / 100.0;
        }
        else if(line.contains("Root relative squared error "))
        {
            line.remove("Root relative squared error ");
            line.remove(" %");
            values["rrse"] = line.toDouble() / 100.0;
        }
    }

    return values;
}

void WEKA::analyzeMDI(const QString &_raw, const QString &_file)
{
    QString importance = _raw.split("Attribute importance based on average impurity decrease (and number of nodes using that attribute)").last();
    importance = importance.split("Time taken to build model").first();

    QMap<QString, double> result;
    QStringList lines = importance.split("\n");
    for(int i=0; i<lines.size(); i++)
    {
        QString line = lines.at(i);
        if(line.contains(" (") && line.contains(") "))
        {
            double value = line.split(" (").first().toDouble();
            QString key = line.split(")  ").last();

            result[key] = value;
        }
    }

    //
    QString data;
    QTextStream stream(&data);

    double sum = 0;
    for(QMap<QString, double>::iterator it=result.begin(); it!=result.end(); it++)
        sum += it.value();

    int index = 1;
    for(QMap<QString, double>::iterator it=result.begin(); it!=result.end(); it++)
    {
        stream << it.value()/sum << ",";
        //qDebug() << index++ << it.key();
    }

    QString path = _file;
    path = path.replace(".csv", "_rf_importance.txt");
    write(data, path);
}

void WEKA::optimizeANN(const QString &_file)
{
    QList<double> rates = QList<double>() << 0.001 << 0.1 << 0.2 << 0.3;
    QList<double> momentums = QList<double>() << 0.001 << 0.1 << 0.2 << 0.3;

    double r2_max = 0;
    QString bestModel = "";
    foreach(double rate, rates)
    {
        foreach(double momentum, momentums)
        {
            QString model = "weka.classifiers.functions.MultilayerPerceptron -L " + QString::number(rate) + " -M " + QString::number(momentum) + " -N 500 -V 0 -S 0 -E 20 -H \"10,5\"";
            QMap<QString, double> result = runModel(_file, model);

            qDebug() << rate << momentum << result;

            double corr = result["corr"];
            double r2 = corr * corr;

            if(r2>r2_max)
            {
                r2_max = r2;
                bestModel = model;

                qDebug() << "best model" << bestModel;
            }
        }
    }

}

QStringList WEKA::read(const QString &_file)
{
    QStringList lines;

    QFile file(_file);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        while(!in.atEnd())
            lines << in.readLine();

        file.close();
    }

    return lines;
}

void WEKA::write(const QString &_data, const QString &_file)
{
    QFile file(_file);
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        stream << _data;
        file.close();
    }
}

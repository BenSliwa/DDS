#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QDir>
#include <QDateTime>
#include "data/dataparser.h"
#include "cat/cat.h"

void evaluate(const QString &_scenario)
{
    CAT cat;
    QString folder = "MultiCarrier/" + _scenario + "/";
    QString results = "";

    QDir directory(folder);
    QStringList files = directory.entryList(QStringList() << "*_tmobile_context.txt", QDir::Files);

    cat.handleTraces(folder, files, results);
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    evaluate("suburban");
    evaluate("highway");


    return app.exec();
}

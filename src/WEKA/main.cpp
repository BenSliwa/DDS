#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QProcess>
#include <QDebug>
#include <QDir>
#include "weka.h"
#include "randomforest.h"



int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    WEKA weka;




    return app.exec();
}

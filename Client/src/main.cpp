#include <QtGui>
#include <QApplication>
#include "uimanager.h"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    UiManager *ui = UiManager::getInstance();




    return app.exec();
}

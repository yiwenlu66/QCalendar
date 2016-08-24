#include <QApplication>
#include <QTranslator>
#include <QtDebug>
#include "mainwindow.h"
#include "configloader.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ConfigLoader* config = new ConfigLoader;

    MainWindow mainWindow(&app, config);
    mainWindow.show();

    return app.exec();
}

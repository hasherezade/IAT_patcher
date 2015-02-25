#include "mainwindow.h"
#include <QApplication>
#include <QtGui>

#include <iostream>
#include <stdio.h>
#include <bearparser.h>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(application);
    QApplication app(argc, argv);
    ExeFactory::init();

    MainWindow w;
    w.show();

    return app.exec();
}

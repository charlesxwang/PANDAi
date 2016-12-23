#include <QQmlContext>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "bluelinker.h"
#include <cstdio>
#include <iostream>
#include "HandDetector.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    BlueLinker blueLinker;

    HandDetector handdetector;
    blueLinker.m_num = &handdetector.m_number;
    blueLinker.m_string = &handdetector.m_string;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("blueLinker", &blueLinker);

    engine.load(QUrl("qrc:/assets/main.qml"));
    handdetector.run();
    return app.exec();
}

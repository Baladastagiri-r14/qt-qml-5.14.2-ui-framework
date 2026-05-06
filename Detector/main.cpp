#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "iRay.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    iRay irayObject;  // Create instance

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("iray", &irayObject);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

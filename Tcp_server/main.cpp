//#include <QGuiApplication>
//#include <QQmlApplicationEngine>
//#include <QQmlContext>
//#include <QSharedPointer>
//#include "esp32camstream.h"
//#include "frametcpserver.h"
//#include <QQuickImageProvider>


//class LiveImageProvider : public QQuickImageProvider
//{
//public:
//    LiveImageProvider(ESP32CamStream* stream)
//        : QQuickImageProvider(QQuickImageProvider::Image), m_stream(stream) {}

//    QImage requestImage(const QString &, QSize *size, const QSize &) override {
//        QImage frame = m_stream->frame();
//        if (size) *size = frame.size();
//        return frame;
//    }

//private:
//    ESP32CamStream* m_stream;
//};

//int main(int argc, char *argv[])
//{
//    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//    QGuiApplication app(argc, argv);

//    ESP32CamStream stream;
//    stream.setRunning(true);  // Start streaming right away

//    QQmlApplicationEngine engine;

//    auto imageProvider = QSharedPointer<LiveImageProvider>(new LiveImageProvider(&stream));
//    engine.addImageProvider("live", imageProvider.data());
//    engine.rootContext()->setContextProperty("camStream", &stream);

//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
//    if (engine.rootObjects().isEmpty())
//        return -1;

//    // Start TCP server
//    FrameTcpServer tcpServer(&stream);
//    tcpServer.startServer(9999);  // TCP stream on port 9999

//    return app.exec();
//}

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "esp32camstream.h"
#include "frametcpserver.h"
#include <QQuickImageProvider>

class LiveImageProvider : public QQuickImageProvider {
public:
    LiveImageProvider(ESP32CamStream* stream)
        : QQuickImageProvider(QQuickImageProvider::Image), m_stream(stream) {}

    QImage requestImage(const QString &, QSize *size, const QSize &) override {
        QImage frame = m_stream->frame();
        if (size) *size = frame.size();
        return frame;
    }

private:
    ESP32CamStream* m_stream;
};

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    ESP32CamStream stream;
    stream.setRunning(true);

    // Must create TCP server on heap to avoid deletion issues
    FrameTcpServer* tcpServer = new FrameTcpServer(&stream, &app);
    tcpServer->startServer(9999);

    QQmlApplicationEngine engine;

    // Register the TCP server instance
    engine.rootContext()->setContextProperty("tcpServer", tcpServer);
    engine.rootContext()->setContextProperty("camStream", &stream);

    engine.addImageProvider("live", new LiveImageProvider(&stream));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

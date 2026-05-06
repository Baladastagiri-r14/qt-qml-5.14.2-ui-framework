#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickImageProvider>
#include <QSharedPointer>
#include "esp32camstream.h"

class LiveImageProvider : public QQuickImageProvider
{
public:
    LiveImageProvider(ESP32CamStream* stream)
        : QQuickImageProvider(QQuickImageProvider::Image),
          m_stream(stream) {}

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override
    {
        Q_UNUSED(id)
        Q_UNUSED(requestedSize)

        QImage frame = m_stream->frame();
        if (size) *size = frame.size();
        return frame;
    }

//private:
//    ESP32CamStream* m_stream;
//};
//class LiveImageProvider : public QQuickImageProvider
//{
//public:
//    LiveImageProvider(ESP32CamStream* stream)
//        : QQuickImageProvider(QQuickImageProvider::Image),
//          m_stream(stream) {}

//    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override
//    {
//        Q_UNUSED(id)

//        QImage frame = m_stream->frame();

//        if (frame.isNull()) {
//            qWarning() << "LiveImageProvider: Frame is null!";
//            if (size) *size = QSize(0, 0);
//            return QImage();
//        }

//        // Log size for debug
//        qDebug() << "LiveImageProvider: Frame size:" << frame.size();

//        QRect cropRect(120, 50, 50, 190);
////      QRect cropRect(110, 0, 50, 240);


//        // Check if the frame is large enough
//        if (!QRect(QPoint(0, 0), frame.size()).contains(cropRect)) {
//            qWarning() << "LiveImageProvider: Frame too small to crop. Size:"
//                       << frame.size() << "CropRect:" << cropRect;
//            if (size) *size = frame.size();
//            return frame; // fallback: return full frame instead of cropped
//        }

//        // Perform cropping
//        QImage cropped = frame.copy(cropRect);

//        if (requestedSize.isValid()) {
//            cropped = cropped.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//        }

//        if (size)
//            *size = cropped.size();

//        return cropped;
//    }



//    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override
//    {
//        Q_UNUSED(id)

//        QImage frame = m_stream->frame();

//        if (frame.isNull()) {
//            qWarning() << "LiveImageProvider: Frame is null!";
//            if (size) *size = QSize(0, 0);
//            return QImage();
//        }

//        qDebug() << "LiveImageProvider: Frame size:" << frame.size();

//        QRect cropRect(130, 50, 50, 155); // Your crop rectangle

//        if (!QRect(QPoint(0, 0), frame.size()).contains(cropRect)) {
//            qWarning() << "LiveImageProvider: Frame too small to crop. Size:"
//                       << frame.size() << " CropRect:" << cropRect;
//            if (size) *size = frame.size();
//            return frame;
//        }

//        // Crop region from original frame
//        QImage cropped = frame.copy(cropRect);

//        // Now upscale cropped image to full original frame size
//        QSize fullSize = frame.size(); // i.e., 320x240

//        QImage upscaled = cropped.scaled(fullSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

//        if (size)
//            *size = upscaled.size();

//        return upscaled;
//    }

private:
    ESP32CamStream* m_stream;
};


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    ESP32CamStream stream;
    QQmlApplicationEngine engine;

    // Use smart pointer for image provider
    auto imageProvider = QSharedPointer<LiveImageProvider>(new LiveImageProvider(&stream));
//    engine.addImageProvider("live", imageProvider.data());
    engine.addImageProvider("live", new LiveImageProvider(&stream));
    engine.rootContext()->setContextProperty("camStream", &stream);

    // Connect error signals to qWarning
    QObject::connect(&stream, &ESP32CamStream::errorOccurred, [](const QString &error) {
        qWarning() << "Stream Error:" << error;
    });

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    // Cleanup on exit
    QObject::connect(&app, &QGuiApplication::aboutToQuit, [&]() {
        stream.setRunning(false);
        engine.removeImageProvider("live");
    });

    return app.exec();
}

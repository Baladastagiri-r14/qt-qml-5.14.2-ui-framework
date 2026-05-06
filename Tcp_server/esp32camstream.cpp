#include "esp32camstream.h"
#include <QBuffer>
#include <QImageReader>
#include <QDebug>

ESP32CamStream::ESP32CamStream(QObject *parent)
    : QObject(parent)
{
    m_frame = QImage(320, 240, QImage::Format_RGB888);
    m_frame.fill(Qt::black);

    m_networkManager = new QNetworkAccessManager(this);

    m_fpsTimer.setInterval(1000);
    connect(&m_fpsTimer, &QTimer::timeout, this, &ESP32CamStream::updateFps);
}

ESP32CamStream::~ESP32CamStream() {
    setRunning(false);
}

QImage ESP32CamStream::frame() const {
    QMutexLocker locker(&m_mutex);
    return m_frame;
}

void ESP32CamStream::setRunning(bool running) {
    if (m_running == running) return;
    m_running = running;

    if (m_running) {
        m_frameCount = 0;
        m_fpsTimer.start();

        QNetworkRequest req(QUrl("http://192.168.4.1:81/stream"));
        req.setRawHeader("Accept", "multipart/x-mixed-replace");
        req.setHeader(QNetworkRequest::UserAgentHeader, "ESP32-CAM-Viewer");

        m_currentReply = m_networkManager->get(req);
        connect(m_currentReply, &QNetworkReply::readyRead, this, &ESP32CamStream::onReadyRead);
    } else {
        m_fpsTimer.stop();
        if (m_currentReply) {
            m_currentReply->abort();
            m_currentReply->deleteLater();
            m_currentReply = nullptr;
        }
        m_buffer.clear();
    }
}

void ESP32CamStream::onReadyRead() {
    m_buffer.append(m_currentReply->readAll());
    processBuffer();
}

void ESP32CamStream::processBuffer() {
    QMutexLocker locker(&m_mutex);

    while (true) {
        int start = m_buffer.indexOf("\xFF\xD8");
        int end = m_buffer.indexOf("\xFF\xD9", start + 2);
        if (start < 0 || end < 0 || end <= start) break;

        QByteArray jpg = m_buffer.mid(start, end - start + 2);
        m_buffer.remove(0, end + 2);

        QImage img;
        if (img.loadFromData(jpg, "JPEG")) {
            m_frame = img.convertToFormat(QImage::Format_RGB888);
            m_frameCount++;
            emit frameChanged();
        }
    }
}

void ESP32CamStream::updateFps() {
    qDebug() << "FPS:" << m_frameCount;
    m_frameCount = 0;
}


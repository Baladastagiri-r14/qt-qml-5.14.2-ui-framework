#include "esp32camstream.h"
#include <QDebug>
#include <QDateTime>
#include <QUrlQuery>

ESP32CamStream::ESP32CamStream(QObject *parent) : QObject(parent),
    m_mutex(QMutex::Recursive)
{
    m_frame = QImage(320, 240, QImage::Format_RGB888);
    m_frame.fill(Qt::black);

    m_networkManager = new QNetworkAccessManager(this);
    connect(m_networkManager, &QNetworkAccessManager::finished,
            this, &ESP32CamStream::handleNetworkData);

    m_fpsTimer.setInterval(1000);
    connect(&m_fpsTimer, &QTimer::timeout, this, &ESP32CamStream::updateFps);

    m_settingsApplyTimer.setInterval(500);
    m_settingsApplyTimer.setSingleShot(true);
    connect(&m_settingsApplyTimer, &QTimer::timeout, this, &ESP32CamStream::applyCameraSettings);
}

ESP32CamStream::~ESP32CamStream()
{
    setRunning(false);
    m_networkManager->deleteLater();
}

// Basic property getters
QImage ESP32CamStream::frame() const {
    QMutexLocker locker(&m_mutex);
    return m_frame;
}

bool ESP32CamStream::running() const { return m_running; }
QString ESP32CamStream::streamUrl() const { return m_streamUrl; }
int ESP32CamStream::fps() const { return m_fps; }

// Image property getters
int ESP32CamStream::framesize() const { return m_framesize; }
int ESP32CamStream::quality() const { return m_quality; }
int ESP32CamStream::brightness() const { return m_brightness; }
int ESP32CamStream::contrast() const { return m_contrast; }
int ESP32CamStream::saturation() const { return m_saturation; }
int ESP32CamStream::specialEffect() const { return m_specialEffect; }
int ESP32CamStream::gainCeiling() const { return m_gainCeiling; }
bool ESP32CamStream::wpc() const { return m_wpc; }
bool ESP32CamStream::hmirror() const { return m_hmirror; }
bool ESP32CamStream::vflip() const { return m_vflip; }

// Basic property setters
void ESP32CamStream::setRunning(bool running) {
    if (m_running == running) return;

    QMutexLocker locker(&m_mutex);
    m_running = running;

    if (m_running) {
        m_frameCount = 0;
        m_fpsTimer.start();
        m_restartPending = false;
        startStream();
    } else {
        m_fpsTimer.stop();
        stopStream();
    }

    emit runningChanged(m_running);
}

void ESP32CamStream::setStreamUrl(QString streamUrl) {
    if (m_streamUrl == streamUrl) return;

    QMutexLocker locker(&m_mutex);
    m_streamUrl = streamUrl;
    emit streamUrlChanged(m_streamUrl);

    if (m_running) {
        stopStream();
        startStream();
    }
}

// Image property setters
void ESP32CamStream::setFramesize(int value) {
    if (m_framesize != value) {
        m_framesize = qBound(0, value, 10);
        emit framesizeChanged(m_framesize);
        m_settingsApplyTimer.start();
    }
}

void ESP32CamStream::setQuality(int value) {
    if (m_quality != value) {
        m_quality = qBound(0, value, 63);
        emit qualityChanged(m_quality);
        m_settingsApplyTimer.start();
    }
}

void ESP32CamStream::setBrightness(int value) {
    if (m_brightness != value) {
        m_brightness = qBound(-2, value, 2);
        emit brightnessChanged(m_brightness);
        m_settingsApplyTimer.start();
    }
}

void ESP32CamStream::setContrast(int value) {
    if (m_contrast != value) {
        m_contrast = qBound(-2, value, 2);
        emit contrastChanged(m_contrast);
        m_settingsApplyTimer.start();
    }
}

void ESP32CamStream::setSaturation(int value) {
    if (m_saturation != value) {
        m_saturation = qBound(-2, value, 2);
        emit saturationChanged(m_saturation);
        m_settingsApplyTimer.start();
    }
}

void ESP32CamStream::setSpecialEffect(int value) {
    if (m_specialEffect != value) {
        m_specialEffect = qBound(0, value, 6);
        emit specialEffectChanged(m_specialEffect);
        m_settingsApplyTimer.start();
    }
}

void ESP32CamStream::setGainCeiling(int ceiling) {
    if (m_gainCeiling != ceiling) {
        m_gainCeiling = qBound(0, ceiling, 6);
        emit gainCeilingChanged(m_gainCeiling);
        m_settingsApplyTimer.start();
    }
}

void ESP32CamStream::setWpc(bool enabled) {
    if (m_wpc != enabled) {
        m_wpc = enabled;
        emit wpcChanged(m_wpc);
        m_settingsApplyTimer.start();
    }
}

void ESP32CamStream::setHmirror(bool enabled) {
    if (m_hmirror != enabled) {
        m_hmirror = enabled;
        emit hmirrorChanged(m_hmirror);
        m_settingsApplyTimer.start();
    }
}

void ESP32CamStream::setVflip(bool enabled) {
    if (m_vflip != enabled) {
        m_vflip = enabled;
        emit vflipChanged(m_vflip);
        m_settingsApplyTimer.start();
    }
}

void ESP32CamStream::applyCameraSettings() {
    if (!m_running) return;

    QString controlUrl = m_streamUrl;
    if (controlUrl.endsWith("/stream")) {
        controlUrl.replace("/stream", "/control");
    } else if (!controlUrl.endsWith("/control")) {
        controlUrl += "/control";
    }

    QUrl url(controlUrl);
    QUrlQuery query;

    query.addQueryItem("var", "framesize");
    query.addQueryItem("val", QString::number(m_framesize));

    query.addQueryItem("var", "quality");
    query.addQueryItem("val", QString::number(m_quality));

    query.addQueryItem("var", "brightness");
    query.addQueryItem("val", QString::number(m_brightness));

    query.addQueryItem("var", "contrast");
    query.addQueryItem("val", QString::number(m_contrast));

    query.addQueryItem("var", "saturation");
    query.addQueryItem("val", QString::number(m_saturation));

    query.addQueryItem("var", "special_effect");
    query.addQueryItem("val", QString::number(m_specialEffect));

    query.addQueryItem("var", "gainceiling");
    query.addQueryItem("val", QString::number(m_gainCeiling));

    query.addQueryItem("var", "wpc");
    query.addQueryItem("val", m_wpc ? "1" : "0");

    query.addQueryItem("var", "hmirror");
    query.addQueryItem("val", m_hmirror ? "1" : "0");

    query.addQueryItem("var", "vflip");
    query.addQueryItem("val", m_vflip ? "1" : "0");

    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            this, [](QNetworkReply::NetworkError code) {
        Q_UNUSED(code)
        qDebug() << "Failed to apply camera settings";
    });
}

void ESP32CamStream::startStream()
{
    QMutexLocker locker(&m_mutex);

    if (m_currentReply) {
        return;
    }

    qDebug() << "Starting stream from:" << m_streamUrl;
    m_buffer.clear();
    m_processing = false;

    QNetworkRequest request;
    request.setUrl(QUrl(m_streamUrl));
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    request.setRawHeader("Accept", "multipart/x-mixed-replace");
    request.setHeader(QNetworkRequest::UserAgentHeader, "ESP32-CAM-Viewer");

    m_currentReply = m_networkManager->get(request);

    connect(m_currentReply, &QNetworkReply::readyRead, this, [this]() {
        if (!m_processing) {
            m_processing = true;
            QMutexLocker locker(&m_mutex);
            if (m_currentReply) {
                m_buffer.append(m_currentReply->readAll());
                processBuffer();
            }
            m_processing = false;
        }
    });

    // Handle network errors - COMPATIBILITY FIX for Qt < 5.15
    connect(m_currentReply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            this, [this](QNetworkReply::NetworkError code) {
        Q_UNUSED(code)
        if (m_currentReply && m_currentReply->error() != QNetworkReply::OperationCanceledError) {
            emit errorOccurred(QString("Network error: %1").arg(m_currentReply->errorString()));
            qDebug() << "Network error:" << m_currentReply->errorString();
            restartIfNeeded();
        }
    });

    // Conservative timeout (only if no data at all)
    QTimer::singleShot(15000, this, [this]() {
        QMutexLocker locker(&m_mutex);
        if (m_currentReply && m_currentReply->isRunning() && m_frameCount == 0) {
            qDebug() << "No frames received - restarting stream";
            emit errorOccurred("No frames received - restarting");
            stopStream();
            restartIfNeeded();
        }
    });
}

void ESP32CamStream::stopStream()
{
    QMutexLocker locker(&m_mutex);

    if (m_currentReply) {
        disconnect(m_currentReply, nullptr, this, nullptr);
        m_currentReply->abort();
        m_currentReply->deleteLater();
        m_currentReply = nullptr;
    }
    m_buffer.clear();
}

void ESP32CamStream::restartIfNeeded()
{
    QMutexLocker locker(&m_mutex);
    if (m_running && !m_restartPending) {
        m_restartPending = true;
        QTimer::singleShot(1000, this, [this]() {
            QMutexLocker locker(&m_mutex);
            m_restartPending = false;
            if (m_running) {
                startStream();
            }
        });
    }
}

void ESP32CamStream::updateFps()
{
    QMutexLocker locker(&m_mutex);
    m_fps = m_frameCount;
    m_frameCount = 0;
    emit fpsChanged(m_fps);
}

void ESP32CamStream::handleNetworkData(QNetworkReply *reply)
{
    if (reply != m_currentReply) {
        reply->deleteLater();
        return;
    }

    if (reply->error() != QNetworkReply::NoError) {
        if (reply->error() != QNetworkReply::OperationCanceledError) {
            emit errorOccurred(QString("Stream error: %1").arg(reply->errorString()));
            qDebug() << "Stream error:" << reply->errorString();
        }
        restartIfNeeded();
    }
    reply->deleteLater();
}

void ESP32CamStream::processBuffer()
{
    QMutexLocker locker(&m_mutex);

    while (m_buffer.size() > 1024) {
        int start = m_buffer.indexOf("\xff\xd8");
        if (start < 0) {
            if (m_buffer.size() > 4096) {
                m_buffer.clear();
            }
            return;
        }

        int end = m_buffer.indexOf("\xff\xd9", start + 2);
        if (end < start) {
            if (start > 0) {
                m_buffer.remove(0, start);
            }
            return;
        }

        QByteArray jpgData = m_buffer.mid(start, end - start + 2);
        m_buffer.remove(0, end + 2);

        QImage image;
        if (image.loadFromData(jpgData, "JPEG")) {
            if (image.size() != QSize(320, 240) || image.format() != QImage::Format_RGB888) {
                image = image.convertToFormat(QImage::Format_RGB888)
                        .scaled(320, 240, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            }

            m_frame = image.copy();
            m_frameCount++;
            emit frameChanged(m_frame);
        }
    }
}

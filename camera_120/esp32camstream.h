#ifndef ESP32CAMSTREAM_H
#define ESP32CAMSTREAM_H

#include <QObject>
#include <QImage>
#include <QMutex>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

class ESP32CamStream : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QImage frame READ frame NOTIFY frameChanged)
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(QString streamUrl READ streamUrl WRITE setStreamUrl NOTIFY streamUrlChanged)
    Q_PROPERTY(int fps READ fps NOTIFY fpsChanged)

    // Image Properties used in QML
    Q_PROPERTY(int framesize READ framesize WRITE setFramesize NOTIFY framesizeChanged)
    Q_PROPERTY(int quality READ quality WRITE setQuality NOTIFY qualityChanged)
    Q_PROPERTY(int brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged)
    Q_PROPERTY(int contrast READ contrast WRITE setContrast NOTIFY contrastChanged)
    Q_PROPERTY(int saturation READ saturation WRITE setSaturation NOTIFY saturationChanged)
    Q_PROPERTY(int specialEffect READ specialEffect WRITE setSpecialEffect NOTIFY specialEffectChanged)
    Q_PROPERTY(int gainCeiling READ gainCeiling WRITE setGainCeiling NOTIFY gainCeilingChanged)
    Q_PROPERTY(bool wpc READ wpc WRITE setWpc NOTIFY wpcChanged)
    Q_PROPERTY(bool hmirror READ hmirror WRITE setHmirror NOTIFY hmirrorChanged)
    Q_PROPERTY(bool vflip READ vflip WRITE setVflip NOTIFY vflipChanged)

public:
    explicit ESP32CamStream(QObject *parent = nullptr);
    ~ESP32CamStream();

    QImage frame() const;
    bool running() const;
    QString streamUrl() const;
    int fps() const;

    // Image properties
    int framesize() const;
    int quality() const;
    int brightness() const;
    int contrast() const;
    int saturation() const;
    int specialEffect() const;
    int gainCeiling() const;
    bool wpc() const;
    bool hmirror() const;
    bool vflip() const;

public slots:
    void setRunning(bool running);
    void setStreamUrl(QString streamUrl);

    // Image property setters
    void setFramesize(int value);
    void setQuality(int value);
    void setBrightness(int value);
    void setContrast(int value);
    void setSaturation(int value);
    void setSpecialEffect(int value);
    void setGainCeiling(int ceiling);
    void setWpc(bool enabled);
    void setHmirror(bool enabled);
    void setVflip(bool enabled);

    void applyCameraSettings();

signals:
    void frameChanged(QImage frame);
    void runningChanged(bool running);
    void streamUrlChanged(QString streamUrl);
    void fpsChanged(int fps);
    void errorOccurred(QString error);

    // Image property signals
    void framesizeChanged(int framesize);
    void qualityChanged(int quality);
    void brightnessChanged(int brightness);
    void contrastChanged(int contrast);
    void saturationChanged(int saturation);
    void specialEffectChanged(int effect);
    void gainCeilingChanged(int ceiling);
    void wpcChanged(bool enabled);
    void hmirrorChanged(bool enabled);
    void vflipChanged(bool enabled);

private slots:
    void handleNetworkData(QNetworkReply *reply);
    void restartIfNeeded();
    void updateFps();

private:
    void processBuffer();
    void startStream();
    void stopStream();

    // Basic members
    QImage m_frame;
    bool m_running = false;
    bool m_processing = false;
    bool m_restartPending = false;
    QString m_streamUrl = "http://192.168.4.1:81/stream";
    int m_frameCount = 0;
    int m_fps = 0;

    // Camera settings used in QML
    int m_framesize = 10;
    int m_quality = 10;
    int m_brightness = 0;
    int m_contrast = 0;
    int m_saturation = 0;
    int m_specialEffect = 0;
    int m_gainCeiling = 0;
    bool m_wpc = true;
    bool m_hmirror = false;
    bool m_vflip = false;

    QNetworkAccessManager *m_networkManager = nullptr;
    QNetworkReply *m_currentReply = nullptr;
    QByteArray m_buffer;
    mutable QMutex m_mutex;
    QTimer m_fpsTimer;
    QTimer m_settingsApplyTimer;
};

#endif // ESP32CAMSTREAM_H

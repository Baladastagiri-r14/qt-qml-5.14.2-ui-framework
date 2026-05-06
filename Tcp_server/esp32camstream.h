#ifndef ESP32CAMSTREAM_H
#define ESP32CAMSTREAM_H

#include <QObject>
#include <QImage>
#include <QMutex>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

class ESP32CamStream : public QObject {
    Q_OBJECT
    Q_PROPERTY(QImage frame READ frame NOTIFY frameChanged)
public:
    explicit ESP32CamStream(QObject *parent = nullptr);
    ~ESP32CamStream();

    QImage frame() const;
    void setRunning(bool running);

signals:
    void frameChanged();

private slots:
    void onReadyRead();
    void processBuffer();
    void updateFps();

private:
    bool m_running = false;
    QImage m_frame;
    QByteArray m_buffer;
    QNetworkAccessManager *m_networkManager = nullptr;
    QNetworkReply *m_currentReply = nullptr;
    QTimer m_fpsTimer;
    int m_frameCount = 0;
     mutable QMutex m_mutex;
};

#endif // ESP32CAMSTREAM_H

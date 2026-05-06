#ifndef ESP32SERVER_H
#define ESP32SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QImage>
#include <QTimer>

class ESP32Server : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QImage currentFrame READ currentFrame NOTIFY frameUpdated)

public:
    explicit ESP32Server(QObject *parent = nullptr);
    ~ESP32Server();

    QImage currentFrame() const;

public slots:
    void startServer(int port = 9999);

signals:
    void frameUpdated();

private slots:
    void onNewConnection();
    void onDataReceived();

private:
    QTcpServer *server;
    QTcpSocket *clientSocket;
    QByteArray buffer;
    QImage frame;

    double readTimestamp(const QByteArray &data);
};

#endif // ESP32SERVER_H

//#ifndef FRAMETCPSERVER_H
//#define FRAMETCPSERVER_H

//#include <QTcpServer>
//#include <QTcpSocket>
//#include <QTimer>
//#include "esp32camstream.h"

//class FrameTcpServer : public QObject
//{
//    Q_OBJECT

//public:
//    FrameTcpServer(ESP32CamStream *stream, QObject *parent = nullptr);
//    void startServer(quint16 port = 9999);

//private slots:
//    void onNewConnection();
//    void sendFrame();

//private:
//    QTcpServer m_server;
//    QTcpSocket *m_clientSocket = nullptr;
//    ESP32CamStream *m_stream;
//    QTimer m_sendTimer;
//};

//#endif // FRAMETCPSERVER_H
#ifndef FRAMETCPSERVER_H
#define FRAMETCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QMutex>
#include "esp32camstream.h"

class FrameTcpServer : public QObject
{
    Q_OBJECT
     Q_PROPERTY(bool clientConnected READ isClientConnected NOTIFY clientConnectedChanged)
public:
    explicit FrameTcpServer(ESP32CamStream *stream, QObject *parent = nullptr);
    void startServer(quint16 port = 9999);
    Q_INVOKABLE void sendDataToClient(const QByteArray &data);
    Q_INVOKABLE bool isClientConnected() const;

signals:
    void receivedData(const QByteArray &data);
    void clientConnected();
    void clientDisconnected();
    void clientConnectedChanged(bool connected);

private slots:
    void onNewConnection();
    void sendFrame();
    void onClientReadyRead();
    void onClientDisconnected();  // Add this missing declaration

private:
    QTcpServer m_server;
    QTcpSocket *m_clientSocket = nullptr;
    ESP32CamStream *m_stream;
    QTimer m_sendTimer;
    mutable QMutex m_mutex;
};

#endif // FRAMETCPSERVER_H

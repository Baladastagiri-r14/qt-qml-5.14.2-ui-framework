//#include "frametcpserver.h"
//#include <QBuffer>
//#include <QDataStream>
//#include <QDateTime>
//#include <QDebug>

//FrameTcpServer::FrameTcpServer(ESP32CamStream *stream, QObject *parent)
//    : QObject(parent), m_stream(stream)
//{
//    connect(&m_server, &QTcpServer::newConnection, this, &FrameTcpServer::onNewConnection);
//    connect(&m_sendTimer, &QTimer::timeout, this, &FrameTcpServer::sendFrame);
//    m_sendTimer.setInterval(40);  // ~25 FPS
//}

//void FrameTcpServer::startServer(quint16 port)
//{
//    if (!m_server.listen(QHostAddress::Any, port)) {
//        qWarning() << "TCP server failed to start:" << m_server.errorString();
//    } else {
//        qDebug() << "TCP server listening on port" << port;
//    }
//}

//void FrameTcpServer::onNewConnection()
//{
//    if (m_clientSocket) {
//        m_clientSocket->disconnectFromHost();
//        m_clientSocket->deleteLater();
//    }

//    m_clientSocket = m_server.nextPendingConnection();
//    qDebug() << "Client connected from" << m_clientSocket->peerAddress().toString();

//    connect(m_clientSocket, &QTcpSocket::disconnected, this, [this]() {
//        qDebug() << "Client disconnected";
//        m_clientSocket->deleteLater();
//        m_clientSocket = nullptr;
//        m_sendTimer.stop();
//    });

//    m_sendTimer.start();
//}

//void FrameTcpServer::sendFrame()
//{
//    if (!m_clientSocket || !m_clientSocket->isWritable()) return;

//    QImage frame = m_stream->frame();
//    if (frame.isNull()) return;

//    QByteArray jpegData;
//    QBuffer buffer(&jpegData);
//    buffer.open(QIODevice::WriteOnly);
//    frame.save(&buffer, "JPG", 80);  // Quality: 80

//    if (jpegData.size() < 100) return;

//    qint64 timestamp = QDateTime::currentMSecsSinceEpoch();

//    QByteArray packet;
//    QDataStream stream(&packet, QIODevice::WriteOnly);
//    stream.setByteOrder(QDataStream::BigEndian);

//    stream << static_cast<qint64>(timestamp);
//    stream << static_cast<quint32>(jpegData.size());
//    packet.append(jpegData);

//    m_clientSocket->write(packet);
//    m_clientSocket->flush();
//}

#include "frametcpserver.h"
#include <QBuffer>
#include <QDataStream>
#include <QDateTime>
#include <QDebug>

FrameTcpServer::FrameTcpServer(ESP32CamStream *stream, QObject *parent)
    : QObject(parent), m_stream(stream)
{
    connect(&m_server, &QTcpServer::newConnection, this, &FrameTcpServer::onNewConnection);
    connect(&m_sendTimer, &QTimer::timeout, this, &FrameTcpServer::sendFrame);
    m_sendTimer.setInterval(40);  // ~25 FPS
    emit clientConnectedChanged(false);
}

void FrameTcpServer::startServer(quint16 port)
{
    if (!m_server.listen(QHostAddress::Any, port)) {
        qWarning() << "TCP server failed to start:" << m_server.errorString();
    } else {
        qDebug() << "TCP server listening on port" << port;
    }
}

bool FrameTcpServer::isClientConnected() const
{
    QMutexLocker locker(&m_mutex);
    return m_clientSocket && m_clientSocket->state() == QTcpSocket::ConnectedState;
}

//void FrameTcpServer::onNewConnection()
//{
//    QMutexLocker locker(&m_mutex);

//    if (m_clientSocket) {
//        m_clientSocket->disconnectFromHost();
//        m_clientSocket->deleteLater();
//    }

//    m_clientSocket = m_server.nextPendingConnection();
//    qDebug() << "Client connected from" << m_clientSocket->peerAddress().toString();

//    connect(m_clientSocket, &QTcpSocket::readyRead, this, &FrameTcpServer::onClientReadyRead);
//    connect(m_clientSocket, &QTcpSocket::disconnected, this, &FrameTcpServer::onClientDisconnected);

//    m_sendTimer.start();
//    emit clientConnected();
//}

//void FrameTcpServer::onClientDisconnected()
//{
//    QMutexLocker locker(&m_mutex);
//    qDebug() << "Client disconnected";
//    if (m_clientSocket) {
//        m_clientSocket->deleteLater();
//        m_clientSocket = nullptr;
//    }
//    m_sendTimer.stop();
//    emit clientDisconnected();
//}

void FrameTcpServer::onNewConnection()
{
    QMutexLocker locker(&m_mutex);

    if (m_clientSocket) {
        m_clientSocket->disconnectFromHost();
        m_clientSocket->deleteLater();
    }

    m_clientSocket = m_server.nextPendingConnection();
    qDebug() << "Client connected from" << m_clientSocket->peerAddress().toString();

    connect(m_clientSocket, &QTcpSocket::readyRead, this, &FrameTcpServer::onClientReadyRead);
    connect(m_clientSocket, &QTcpSocket::disconnected, this, &FrameTcpServer::onClientDisconnected);

    m_sendTimer.start();
    emit clientConnected();
    emit clientConnectedChanged(true); // Add this line
}

void FrameTcpServer::onClientDisconnected()
{
    QMutexLocker locker(&m_mutex);
    qDebug() << "Client disconnected";
    if (m_clientSocket) {
        m_clientSocket->deleteLater();
        m_clientSocket = nullptr;
    }
    m_sendTimer.stop();
    emit clientDisconnected();
    emit clientConnectedChanged(false); // Add this line
}
void FrameTcpServer::sendFrame()
{
    QMutexLocker locker(&m_mutex);

    if (!m_clientSocket || !m_clientSocket->isWritable()) return;

    QImage frame = m_stream->frame();
    if (frame.isNull()) return;

    QByteArray jpegData;
    QBuffer buffer(&jpegData);
    buffer.open(QIODevice::WriteOnly);
    frame.save(&buffer, "JPG", 80);

    if (jpegData.size() < 100) return;

    QByteArray packet;
    QDataStream stream(&packet, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);

    // Packet type 0 = Video Frame
    stream << static_cast<quint8>(0);
    stream << QDateTime::currentMSecsSinceEpoch();
    stream << static_cast<quint32>(jpegData.size());
    packet.append(jpegData);

    m_clientSocket->write(packet);
}

void FrameTcpServer::onClientReadyRead()
{
    QMutexLocker locker(&m_mutex);

    if (!m_clientSocket) return;

    QByteArray data = m_clientSocket->readAll();
    emit receivedData(data);
    qDebug() << "Server received:" << data;
}

void FrameTcpServer::sendDataToClient(const QByteArray &data)
{
    QMutexLocker locker(&m_mutex);

    if (!m_clientSocket || !m_clientSocket->isWritable()) {
        qWarning() << "No connected client to send data to";
        return;
    }

    QByteArray packet;
    QDataStream stream(&packet, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);

    // Packet type 1 = Data Packet
    stream << static_cast<quint8>(1);
    stream << static_cast<quint32>(data.size());
    packet.append(data);

    qint64 bytesWritten = m_clientSocket->write(packet);
    if (bytesWritten == -1) {
        qDebug() << "Failed to write data to client:" << m_clientSocket->errorString();
    } else {
        m_clientSocket->flush();
        qDebug() << "Sent" << bytesWritten << "bytes to client";
    }
}

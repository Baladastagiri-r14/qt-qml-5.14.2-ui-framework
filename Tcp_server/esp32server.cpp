#include "esp32server.h"
#include <QDataStream>
#include <QBuffer>
#include <QDebug>

ESP32Server::ESP32Server(QObject *parent)
    : QObject(parent), server(new QTcpServer(this)), clientSocket(nullptr)
{
    connect(server, &QTcpServer::newConnection, this, &ESP32Server::onNewConnection);
}

ESP32Server::~ESP32Server()
{
    server->close();
}

QImage ESP32Server::currentFrame() const
{
    return frame;
}

void ESP32Server::startServer(int port)
{
    if (!server->listen(QHostAddress::Any, port)) {
        qWarning() << "[Server] Failed to start server on port" << port;
    } else {
        qDebug() << "[Server] Listening on port" << port;
    }
}

void ESP32Server::onNewConnection()
{
    if (clientSocket)
        clientSocket->deleteLater();

    clientSocket = server->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &ESP32Server::onDataReceived);
    connect(clientSocket, &QTcpSocket::disconnected, [this]() {
        qDebug() << "[Server] Client disconnected";
        clientSocket->deleteLater();
        clientSocket = nullptr;
    });

    qDebug() << "[Server] Client connected";
}

void ESP32Server::onDataReceived()
{
    if (!clientSocket) return;

    buffer.append(clientSocket->readAll());

    while (buffer.size() >= 12) {
        double timestamp;
        quint32 length;

        QDataStream stream(buffer);
        stream.setByteOrder(QDataStream::BigEndian);
        stream >> timestamp;
        stream >> length;

        if (buffer.size() < 12 + static_cast<int>(length))
            return;

        QByteArray jpegData = buffer.mid(12, length);
        QImage img;
        img.loadFromData(jpegData, "JPG");

        if (!img.isNull()) {
            frame = img;
            emit frameUpdated();
        }

        buffer = buffer.mid(12 + length);
    }
}

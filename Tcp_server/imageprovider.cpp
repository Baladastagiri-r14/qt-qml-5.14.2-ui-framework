#include "imageprovider.h"

ESP32ImageProvider::ESP32ImageProvider(ESP32Server *server)
    : QQuickImageProvider(QQuickImageProvider::Image), m_server(server)
{
}

QImage ESP32ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QImage image = m_server->currentFrame();
    if (size)
        *size = image.size();
    return image;
}

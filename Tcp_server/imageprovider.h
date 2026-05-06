#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickImageProvider>
#include "esp32server.h"

class ESP32ImageProvider : public QQuickImageProvider
{
public:
    ESP32ImageProvider(ESP32Server *server);

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

private:
    ESP32Server *m_server;
};

#endif // IMAGEPROVIDER_H

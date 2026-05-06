#include "imageprovider.h"

ImageProvider::ImageProvider() : QQuickImageProvider(QQuickImageProvider::Image) {}

void ImageProvider::setImage(const QImage &img) {
    QMutexLocker locker(&mutex);
    image = img.copy();
}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize) {
    QMutexLocker locker(&mutex);
    if (size)
        *size = image.size();
    return image;
}

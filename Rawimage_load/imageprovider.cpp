#include "imageprovider.h"
#include <QImage>
#include <QFile>
#include <QDebug>

imageprovider::imageprovider()
{

}

QImage imageprovider::loadRawImage(const QString &filePath, int width, int height) {
    QImage image(width, height, QImage::Format_RGB888);
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file:" << file.errorString();
        return QImage();
    }

    // Read raw image data
    QByteArray imageData = file.readAll();
    file.close();

    // Convert raw image data to QImage
    int index = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            QRgb pixelValue = qRgb(imageData[index], imageData[index+1], imageData[index+2]);
            image.setPixel(x, y, pixelValue);
            index += 3; // RGB channels
        }
    }

    return image;
}


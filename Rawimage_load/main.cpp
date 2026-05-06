#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QLocale>
#include <QTranslator>
#include <QQmlContext>
#include <QFile>
#include <QDebug>


#include <QQuickImageProvider>

class RawImageProvider : public QQuickImageProvider {
public:
    RawImageProvider(QImage &image) : QQuickImageProvider(QQuickImageProvider::Image), rawImage(image) {
    }

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override {
        Q_UNUSED(id)
        Q_UNUSED(requestedSize)
        if (size) *size = imageSize;
        return rawImage;
    }
private:
    QImage rawImage;
    QSize imageSize;
};

QImage loadRawImage(const QString &filePath, int width, int height) {


    QImage image(width, height, QImage::Format_Grayscale8);

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file:" << file.errorString();
        return QImage();
    }
    file.close();


    FILE* filePtr = fopen(filePath.toStdString().c_str(), "rb");
    if (!filePtr) {
        qDebug() << "Image filePtr" ;
    }


    unsigned short *temp_buffer = new unsigned short[width*height];



    fread(temp_buffer, sizeof(unsigned short), width * height, filePtr);

    unsigned char *ch_img= new unsigned char [width*height];
    for(int i=0;i<width*height;i++)
    {
        ch_img[i]=temp_buffer[i] *255.0/65535.0;
    }
    image = QImage(ch_img,width,height,QImage::Format_Grayscale8);

//    qDebug() << "image" << image.isNull();
    return image;
}
int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    // Load and convert raw image
    QImage rawImage1 =loadRawImage("D:/dastagiri/Rawimage/proj_1.raw", 3072, 3072);

    // Create a QML application engine
    QQmlApplicationEngine engine;

    RawImageProvider rawImageProvider(rawImage1);
        engine.addImageProvider(QLatin1String("rawImage"), &rawImageProvider);


    // Expose the QImage to QML context
    engine.rootContext()->setContextProperty("rawImage", QVariant::fromValue(rawImage1));

    // Load the QML file
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}



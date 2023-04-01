#ifndef QRCODESCANNERQML_H
#define QRCODESCANNERQML_H

#include "../StaticClass/QRCodeScanner.h"

class QRCodeScannerQML: public QObject
{
    Q_OBJECT
public:
    static void initOnce()
    {
        QRCodeScannerQML::instance = new QRCodeScannerQML;
    }

    static QRCodeScannerQML *getInstance()
    {
        return QRCodeScannerQML::instance;
    }

    Q_INVOKABLE QByteArray analysisQRCode(const QImage &image, const char *format, int quality = -1)
    {
        return QRCodeScanner::analysisQRCode(image, format, quality);
    }
    Q_INVOKABLE QByteArray analysisQRCode(const QString &imagePath, const char *format = nullptr, int quality = -1)
    {
        return QRCodeScanner::analysisQRCode(imagePath, format, quality);
    }
    Q_INVOKABLE QByteArray scanQRCodeByTakePhoto()
    {
        return QRCodeScanner::scanQRCodeByTakePhoto();
    }
    Q_INVOKABLE QByteArray scanQRCodeFromPictureFile()
    {
        return QRCodeScanner::scanQRCodeFromPictureFile();
    }
private:
    static QRCodeScannerQML *instance;
    QRCodeScannerQML() = default;
};

#endif // QRCODESCANNERQML_H

#ifndef QRCODESCANNERQML_H
#define QRCODESCANNERQML_H

class QRCodeScannerQML: public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

    Q_INVOKABLE QByteArray analysisQRCode(const QImage &image, const char *format, int quality = -1);
    Q_INVOKABLE QByteArray analysisQRCode(const QString &imagePath, const char *format = nullptr, int quality = -1);

    Q_INVOKABLE QByteArray scanQRCodeByTakePhoto();
    Q_INVOKABLE QByteArray scanQRCodeFromPictureFile();
};

#endif // QRCODESCANNERQML_H

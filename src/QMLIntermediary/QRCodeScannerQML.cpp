#include "QRCodeScannerQML.h"
#include "../StaticClass/QRCodeScanner.h"

QByteArray QRCodeScannerQML::analysisQRCode(const QImage &image, const char *format, int quality)
{
    return QRCodeScanner::analysisQRCode(image, format, quality);
}

QByteArray QRCodeScannerQML::analysisQRCode(const QString &imagePath, const char *format, int quality)
{
    return QRCodeScanner::analysisQRCode(imagePath, format, quality);
}

QByteArray QRCodeScannerQML::scanQRCodeByTakePhoto()
{
    return QRCodeScanner::scanQRCodeByTakePhoto();
}

QByteArray QRCodeScannerQML::scanQRCodeFromPictureFile()
{
    return QRCodeScanner::scanQRCodeByTakePhoto();
}

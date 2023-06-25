#include "QRCodeScannerQML.h"
#include "../Logic/QRCodeScanner.h"

QRCodeScannerQML::QRCodeScannerQML(QObject *parent)
    : QRCodeScanner{parent}
{
}

void QRCodeScannerQML::scanQRCodeFromFile(const QString &filePath)
{
    QImage image;
    image.load(filePath.right(filePath.size() - 8));
    this->scanQRCode(image);
}

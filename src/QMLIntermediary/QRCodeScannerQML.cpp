#include "QRCodeScannerQML.h"
#include "../Logic/QRCodeScanner.h"

QRCodeScannerQML::QRCodeScannerQML(QObject *parent)
    : QRCodeScanner{parent}
{
    connect(this, &QRCodeScannerQML::analysisFinished, [this](const QString & decodeResult)
    {
        emit this->analysisFinishedQML(decodeResult, !decodeResult.isEmpty());
    });
}

void QRCodeScannerQML::readImage(const QVariant &imageVariant)
{
    this->scanQRCode(imageVariant.value<QImage>());
}

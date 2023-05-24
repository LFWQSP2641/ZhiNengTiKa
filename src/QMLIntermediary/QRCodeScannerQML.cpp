#include "QRCodeScannerQML.h"
#include "../Logic/QRCodeScanner.h"

QRCodeScannerQML::QRCodeScannerQML(QObject *parent)
    : QObject{parent}
{
    this->qrCodeScanner = new QRCodeScanner(this);
    connect(qrCodeScanner, &QRCodeScanner::analysisFinished, [this](const QString & decodeResult)
    {
        emit this->analysisFinished(decodeResult, !decodeResult.isEmpty());
    });
    connect(qrCodeScanner, &QRCodeScanner::initializeApiFinished, this, &QRCodeScannerQML::initializeApiFinished);
    connect(qrCodeScanner, &QRCodeScanner::apiInitializing, this, &QRCodeScannerQML::apiInitializing);
}

void QRCodeScannerQML::readImage(const QVariant &imageVariant)
{
    this->qrCodeScanner->scanQRCode(imageVariant.value<QImage>());
}

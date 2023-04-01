#pragma once


namespace QRCodeScanner
{
extern QJsonObject apiArguments;
extern void apiInitialize();

extern void initialize();
extern void initialize(const QJsonObject &apiJsonObject);

extern QByteArray analysisQRCode(const QImage &image, const char *format, int quality = -1);
extern QByteArray analysisQRCode(const QString &imagePath, const char *format = nullptr, int quality = -1);

extern QByteArray scanQRCodeByTakePhoto();
extern QByteArray scanQRCodeFromPictureFile();
};


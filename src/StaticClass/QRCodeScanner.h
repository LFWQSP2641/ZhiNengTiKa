#pragma once


namespace QRCodeScanner
{
extern QJsonObject apiArguments;
extern void apiInitialize();

extern void initialize();
extern void initialize(const QJsonObject &apiJsonObject);

extern QByteArray scanQRCode(const QImage &image, const char *format, int quality = -1);
extern QByteArray scanQRCode(const QString &imagePath, const char *format = nullptr, int quality = -1);
};


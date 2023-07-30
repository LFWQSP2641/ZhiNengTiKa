#ifndef QRSCANLIB_H
#define QRSCANLIB_H

#include "QRScanLib_global.h"
#include <QImage>
#include <QVideoFrame>

class QRSCANLIB_EXPORT QRScanLib
{
public:
    QRScanLib();
    QString decodeImage(const QImage &image, int maxWidth = -1, int maxHeight = -1, bool smoothTransformation = false);
    QString decodeFrame(const QVideoFrame &frame, int maxWidth = -1, int maxHeight = -1, bool smoothTransformation = false);
};

#endif // QRSCANLIB_H

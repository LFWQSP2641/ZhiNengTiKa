#ifndef ZXINGREADER_H
#define ZXINGREADER_H

#include <QImage>
#include <QVideoFrame>
#include "ZXingCpp_global.h"

class ZXINGCPP_EXPORT ZXingReader
{
public:
    static QString decodeImage(const QImage &image, int maxWidth = -1, int maxHeight = -1, bool smoothTransformation = false);
    static QString decodeFrame(const QVideoFrame &frame, int maxWidth = -1, int maxHeight = -1, bool smoothTransformation = false);
private:
    ZXingReader();
};

#endif // ZXINGREADER_H

#ifndef ZXINGREADER_H
#define ZXINGREADER_H

#include <QImage>
#include <QVideoFrame>
#include "ZXingCpp_global.h"
#include "ZXingResult.h"

class ZXINGCPP_EXPORT ZXingReader
{
public:
    static ZXingResult decodeImage(const QImage &image, int maxWidth = -1, int maxHeight = -1, bool smoothTransformation = false);
    static ZXingResult decodeFrame(const QVideoFrame &frame, int maxWidth = -1, int maxHeight = -1, bool smoothTransformation = false);
private:
    ZXingReader();
};

#endif // ZXINGREADER_H

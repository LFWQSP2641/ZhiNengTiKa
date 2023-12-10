#include "ZXingReader.h"
#include "ImageView.h"
#include "ReadBarcode.h"
#include <QDebug>

ZXingReader::ZXingReader()
{
}

ZXingResult ZXingReader::decodeImage(const QImage &image, int maxWidth, int maxHeight, bool smoothTransformation)
{
    if(image.isNull())
    {
        qWarning() << Q_FUNC_INFO << "image is null";
        return ZXingResult();
    }
    QImage qrCodeImage;
    if (((maxWidth > 0) || (maxHeight > 0)) &&
            (image.width() > maxWidth || image.height() > maxHeight))
        qrCodeImage = image.scaled(maxWidth, maxHeight, Qt::KeepAspectRatio, smoothTransformation ? Qt::SmoothTransformation : Qt::FastTransformation);
    else
        qrCodeImage = image;

    ZXing::ImageFormat fmt;

    switch (image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
        fmt = ZXing::ImageFormat::BGRX;
#else
        fmt = ZXing::ImageFormat::XRGB;
#endif
        break;
    case QImage::Format_RGB888:
        fmt = ZXing::ImageFormat::RGB;
        break;
    case QImage::Format_RGBX8888:
    case QImage::Format_RGBA8888:
        fmt = ZXing::ImageFormat::RGBX;
        break;
    case QImage::Format_Grayscale8:
        fmt = ZXing::ImageFormat::Lum;
        break;
    default:
        fmt = ZXing::ImageFormat::None;
        break;
    }
    if(fmt == ZXing::ImageFormat::None)
    {
        qrCodeImage = qrCodeImage.convertToFormat(QImage::Format_Grayscale8);
        fmt = ZXing::ImageFormat::Lum;
    }

    ZXing::ReaderOptions hints;
    hints.setFormats(ZXing::BarcodeFormat::QRCode);

    const auto result(ZXing::ReadBarcode(ZXing::ImageView(
            qrCodeImage.constBits(), qrCodeImage.width(), qrCodeImage.height(),
            fmt), hints));
    const auto position(result.position());
    const auto topLeft(position.topLeft());
    const auto topRight(position.topRight());
    const auto bottomRight(position.bottomRight());
    const auto bottomLeft(position.bottomLeft());
    ZXingResult zxingResult;
    zxingResult.setText(QString::fromStdString(result.text()));
    auto zxingPosition(zxingResult.getPosition());
    zxingPosition.setTopLeft(QPoint(topLeft.x, topLeft.y));
    zxingPosition.setTopRight(QPoint(topRight.x, topRight.y));
    zxingPosition.setBottomRight(QPoint(bottomRight.x, bottomRight.y));
    zxingPosition.setBottomLeft(QPoint(bottomLeft.x, bottomLeft.y));
    return zxingResult;
}

ZXingResult ZXingReader::decodeFrame(const QVideoFrame &frame, int maxWidth, int maxHeight, bool smoothTransformation)
{
    ZXing::ImageFormat fmt = ZXing::ImageFormat::None;
    int pixStride = 0;
    int pixOffset = 0;

    switch (frame.pixelFormat())
    {
    case QVideoFrameFormat::Format_ARGB8888:
    case QVideoFrameFormat::Format_ARGB8888_Premultiplied:
    case QVideoFrameFormat::Format_RGBX8888:
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
        fmt = ZXing::ImageFormat::BGRX;
#else
        fmt = ZXing::ImageFormat::XRGB;
#endif
        break;

    case QVideoFrameFormat::Format_BGRA8888:
    case QVideoFrameFormat::Format_BGRA8888_Premultiplied:
    case QVideoFrameFormat::Format_BGRX8888:
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
        fmt = ZXing::ImageFormat::RGBX;
#else
        fmt = ZXing::ImageFormat::XBGR;
#endif
        break;

    case QVideoFrameFormat::Format_P010:
    case QVideoFrameFormat::Format_P016:
        fmt = ZXing::ImageFormat::Lum, pixStride = 1;
        break;

    case QVideoFrameFormat::Format_AYUV:
    case QVideoFrameFormat::Format_AYUV_Premultiplied:
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
        fmt = ZXing::ImageFormat::Lum, pixStride = 4, pixOffset = 3;
#else
        fmt = ZXing::ImageFormat::Lum, pixStride = 4, pixOffset = 2;
#endif
        break;

    case QVideoFrameFormat::Format_YUV420P:
    case QVideoFrameFormat::Format_NV12:
    case QVideoFrameFormat::Format_NV21:
    case QVideoFrameFormat::Format_IMC1:
    case QVideoFrameFormat::Format_IMC2:
    case QVideoFrameFormat::Format_IMC3:
    case QVideoFrameFormat::Format_IMC4:
    case QVideoFrameFormat::Format_YV12:
        fmt = ZXing::ImageFormat::Lum;
        break;
    case QVideoFrameFormat::Format_UYVY:
        fmt = ZXing::ImageFormat::Lum, pixStride = 2, pixOffset = 1;
        break;
    case QVideoFrameFormat::Format_YUYV:
        fmt = ZXing::ImageFormat::Lum, pixStride = 2;
        break;

    case QVideoFrameFormat::Format_Y8:
        fmt = ZXing::ImageFormat::Lum;
        break;
    case QVideoFrameFormat::Format_Y16:
        fmt = ZXing::ImageFormat::Lum, pixStride = 2, pixOffset = 1;
        break;

    case QVideoFrameFormat::Format_ABGR8888:
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
        fmt = ZXing::ImageFormat::RGBX;
#else
        fmt = ZXing::ImageFormat::XBGR;
#endif
        break;
    case QVideoFrameFormat::Format_YUV422P:
        fmt = ZXing::ImageFormat::Lum;
        break;
    default:
        break;
    }

    ZXing::ReaderOptions hints;
    hints.setFormats(ZXing::BarcodeFormat::QRCode);

    if (fmt != ZXing::ImageFormat::None)
    {
        auto img = frame; // shallow copy just get access to non-const map() function
        if (!img.isValid() || !img.map(QVideoFrame::ReadOnly))
        {
            qWarning() << "invalid QVideoFrame: could not map memory";
            return ZXingResult();
        }
        QScopeGuard unmap([&] { img.unmap(); });

        const auto result(ZXing::ReadBarcode(
        {
            img.bits(0) + pixOffset, img.width(), img.height(), fmt,
            img.bytesPerLine(0), pixStride},
        hints));
        const auto position(result.position());
        const auto topLeft(position.topLeft());
        const auto topRight(position.topRight());
        const auto bottomRight(position.bottomRight());
        const auto bottomLeft(position.bottomLeft());
        ZXingResult zxingResult;
        zxingResult.setText(QString::fromStdString(result.text()));
        auto zxingPosition(zxingResult.getPosition());
        zxingPosition.setTopLeft(QPoint(topLeft.x, topLeft.y));
        zxingPosition.setTopRight(QPoint(topRight.x, topRight.y));
        zxingPosition.setBottomRight(QPoint(bottomRight.x, bottomRight.y));
        zxingPosition.setBottomLeft(QPoint(bottomLeft.x, bottomLeft.y));
        return zxingResult;
    }
    else
    {
        auto qimg = frame.toImage();
        if (qimg.format() != QImage::Format_Invalid)
            return decodeImage(qimg, maxWidth, maxHeight, smoothTransformation);
        qWarning() << "failed to convert QVideoFrame to QImage";
        return ZXingResult();
    }
}

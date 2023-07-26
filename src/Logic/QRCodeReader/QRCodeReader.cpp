#include "QRCodeReader.h"
#include "zxing-cpp/core/src/ReadBarcode.h"

QRCodeReader::QRCodeReader(int maxWidth, int maxHeight, bool smoothTransformation, QObject *parent)
    : QObject{parent}, maxWidth(maxWidth), maxHeight(maxHeight), smoothTransformation(smoothTransformation)
{
    this->hints.setFormats(ZXing::BarcodeFormat::QRCode);
}

QString QRCodeReader::decodeImage(const QImage &image)
{
    if(image.isNull())
    {
        emit decodingFinished(false, {});
        return {};
    }
    QImage qrCodeImage;
    if (((maxWidth > 0) || (maxHeight > 0)) &&
            (image.width() > maxWidth || image.height() > maxHeight))
        qrCodeImage = image.scaled(maxWidth, maxHeight, Qt::KeepAspectRatio, smoothTransformation ? Qt::SmoothTransformation : Qt::FastTransformation);
    else
        qrCodeImage = image;
    try
    {
        const auto result(ZXing::ReadBarcode(ZXing::ImageView(
                qrCodeImage.constBits(), qrCodeImage.width(), qrCodeImage.height(),
                imgFmtFromQImgFmt(qrCodeImage.format())), this->hints));
        const auto resultData(QString::fromStdString(result.text()));
        emit decodingFinished(result.isValid(), resultData);
        return resultData;
    }
    catch(const std::exception& e)
    {
        emit error(QString::fromStdString(e.what()));
    }
    catch(...)
    {
        emit error(QStringLiteral("未知错误"));
    }

    emit decodingFinished(false, {});
    return {};
}

ZXing::ImageFormat QRCodeReader::imgFmtFromQImgFmt(const QImage::Format &qImgFmt)
{
    switch (qImgFmt)
    {
    case QImage::Format::Format_ARGB32:
    case QImage::Format::Format_RGB32:
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
        return ZXing::ImageFormat::BGRX;
#else
        return ZXing::ImageFormat::XRGB;
#endif
    case QImage::Format::Format_RGB888:
        return ZXing::ImageFormat::RGB;
    case QImage::Format::Format_RGBX8888:
    case QImage::Format::Format_RGBA8888:
        return ZXing::ImageFormat::RGBX;
    case QImage::Format::Format_Grayscale8:
        return ZXing::ImageFormat::Lum;
    default:
        return ZXing::ImageFormat::None;
    }
}

bool QRCodeReader::getSmoothTransformation() const
{
    return smoothTransformation;
}

void QRCodeReader::setSmoothTransformation(bool newSmoothTransformation)
{
    if (smoothTransformation == newSmoothTransformation)
        return;
    smoothTransformation = newSmoothTransformation;
    emit smoothTransformationChanged();
}

void QRCodeReader::resetSmoothTransformation()
{
    setSmoothTransformation(false);
}

int QRCodeReader::getMaxHeight() const
{
    return maxHeight;
}

void QRCodeReader::setMaxHeight(int newMaxHeight)
{
    if (maxHeight == newMaxHeight)
        return;
    maxHeight = newMaxHeight;
    emit maxHeightChanged();
}

void QRCodeReader::resetMaxHeight()
{
    setMaxHeight(-1);
}

int QRCodeReader::getMaxWidth() const
{
    return maxWidth;
}

void QRCodeReader::setMaxWidth(int newMaxWidth)
{
    if (maxWidth == newMaxWidth)
        return;
    maxWidth = newMaxWidth;
    emit maxWidthChanged();
}

void QRCodeReader::resetMaxWidth()
{
    setMaxWidth(-1);
}

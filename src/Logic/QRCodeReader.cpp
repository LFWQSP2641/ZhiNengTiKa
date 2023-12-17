#include "QRCodeReader.h"
#include "LibZXingCpp/ZXingReader.h"

QRCodeReader::QRCodeReader(QObject *parent)
    : QObject{parent}
{
}

ZXingResult QRCodeReader::decodeImage(const QImage &image)
{
    if(image.isNull())
    {
        warning(QStringLiteral("图片为空"));
        return {};
    }
    try
    {
        const auto result(ZXingReader::decodeImage(image, maxWidth, maxHeight, smoothTransformation));
        emit decodingFinished(!result.getText().isEmpty(), result);
        return result;
    }
    catch(const std::exception& e)
    {
        warning(QString::fromStdString(e.what()));
        return {};
    }
    catch(...)
    {
        warning(QStringLiteral("未知错误"));
        return {};
    }
}

ZXingResult QRCodeReader::decodeFrame(const QVideoFrame &frame)
{
    if(!frame.isValid())
    {
        frameErrorWarning(QStringLiteral("帧不可用"));
        return {};
    }
    try
    {
        const auto result(ZXingReader::decodeFrame(frame, maxWidth, maxHeight, smoothTransformation));
        emit decodingFinished(!result.getText().isEmpty(), result);
        return result;
    }
    catch(const std::exception& e)
    {
        frameErrorWarning(QString::fromStdString(e.what()));
        return {};
    }
    catch(...)
    {
        frameErrorWarning(QStringLiteral("未知错误"));
        return {};
    }
}

void QRCodeReader::warning(const QString &errMsg)
{
    qWarning() << errMsg;
    emit error(errMsg);
    emit decodingFinished(false, {});
}

void QRCodeReader::frameErrorWarning(const QString &errMsg)
{
    qWarning() << errMsg;
    emit frameError(errMsg);
    emit decodingFinished(false, {});
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

ZXingResult QRCodeReader::decodeImageByPath(const QUrl &imagePath)
{
    auto readablePath(imagePath.toString());
    if(readablePath.startsWith(QStringLiteral("file:///")))
        readablePath = readablePath.right(readablePath.size() - 8);
    return decodeImage(QImage(readablePath));
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

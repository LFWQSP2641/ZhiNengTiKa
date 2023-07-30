#include "QRCodeReader.h"

QRCodeReader::QRCodeReader(QObject *parent)
    : QObject{parent}
{
}

QString QRCodeReader::decodeImage(const QImage &image)
{
    auto warning([this](const QString & errMsg)
    {
        qWarning() << errMsg;
        emit error(errMsg);
        emit decodingFinished(false, {});
    });
    if(image.isNull())
    {
        warning(QStringLiteral("图片为空"));
        return {};
    }
    try
    {
        const auto result(scanner.decodeImage(image, maxWidth, maxHeight, smoothTransformation));
        qDebug() << Q_FUNC_INFO << "result:" << result;
        emit decodingFinished(!result.isEmpty(), result);
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

QString QRCodeReader::decodeFrame(const QVideoFrame &frame)
{
    auto warning([this](const QString & errMsg)
    {
        qWarning() << errMsg;
        emit error(errMsg);
        emit decodingFinished(false, {});
    });
    if(!frame.isValid())
    {
        warning(QStringLiteral("帧不可用"));
        return {};
    }
    try
    {
        const auto result(scanner.decodeFrame(frame, maxWidth, maxHeight, smoothTransformation));
        qDebug() << Q_FUNC_INFO << "result:" << result;
        emit decodingFinished(!result.isEmpty(), result);
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

QString QRCodeReader::decodeImageByPath(const QUrl &imagePath)
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

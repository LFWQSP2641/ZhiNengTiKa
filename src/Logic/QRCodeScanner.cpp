#include "QRCodeScanner.h"

QRCodeScanner::QRCodeScanner(QObject *parent)
    : QRCodeReader{parent}
{
}

QVideoSink *QRCodeScanner::getVideoSink() const
{
    return videoSink;
}

void QRCodeScanner::setVideoSink(QVideoSink *newVideoSink)
{
    if (videoSink == newVideoSink)
        return;
    videoSink = newVideoSink;
    emit videoSinkChanged();
}

void QRCodeScanner::scan()
{
    this->QRCodeReader::decodeFrame(this->videoSink->videoFrame());
}

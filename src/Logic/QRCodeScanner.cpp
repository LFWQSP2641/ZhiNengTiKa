#include "QRCodeScanner.h"

QRCodeScanner::QRCodeScanner(QObject *parent)
    : QThread{parent},
      qrCodeReader(new QRCodeReader)
{
    connect(qrCodeReader, &QRCodeReader::decodingFinished, this, &QRCodeScanner::decodingFinished);
    connect(qrCodeReader, &QRCodeReader::error, this, &QRCodeScanner::error);
    qrCodeReader->moveToThread(this);
}

QRCodeScanner::~QRCodeScanner()
{
    qrCodeReader->deleteLater();
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

bool QRCodeScanner::getAutoStopOnSuccess() const
{
    return autoStopOnSuccess;
}

void QRCodeScanner::setAutoStopOnSuccess(bool newAutoStopOnSuccess)
{
    if (autoStopOnSuccess == newAutoStopOnSuccess)
        return;
    autoStopOnSuccess = newAutoStopOnSuccess;
    emit autoStopOnSuccessChanged();
}

void QRCodeScanner::startThread(Priority priority)
{
    canRun = true;
    this->QThread::start(priority);
}

void QRCodeScanner::stopThread()
{
    canRun = false;
}

void QRCodeScanner::waitForThreadFinish()
{
    this->QThread::wait();
}

void QRCodeScanner::run()
{
    while(canRun)
    {
        if(videoSink == nullptr)
            continue;
        const auto videoFrame(videoSink->videoFrame());
        if(videoFrame.isValid())
        {
            const auto success(!qrCodeReader->decodeFrame(videoFrame)->getText().isEmpty());
            if(success && autoStopOnSuccess)
                break;
        }
    }
}

QRCodeReader *QRCodeScanner::getQrCodeReader() const
{
    return qrCodeReader;
}

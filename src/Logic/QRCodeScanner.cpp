#include "QRCodeScanner.h"
#include "QRCodeReader/QRCodeReader.h"

QRCodeScanner::QRCodeScanner(QObject *parent)
    : QObject{parent},
      reader(new QRCodeReader),
      converter(new VideoFrameConverter),
      thread(new QThread(this))
{
    reader->moveToThread(this->thread);
    connect(reader, &QRCodeReader::decodingFinished, this, &QRCodeScanner::decodingFinished);
    connect(reader, &QRCodeReader::error, this, &QRCodeScanner::error);
    converter->moveToThread(this->thread);
    connect(converter, &VideoFrameConverter::videoFrameChanged, reader, &QRCodeReader::decodeImage);
    thread->start();
}

QRCodeScanner::~QRCodeScanner()
{
    thread->terminate();
    reader->deleteLater();
    converter->deleteLater();
}

QVideoSink *QRCodeScanner::getVideoSink() const
{
    return videoSink;
}

void QRCodeScanner::setVideoSink(QVideoSink *newVideoSink)
{
    if (videoSink == newVideoSink)
        return;
    if(videoSink != nullptr)
        disconnect(videoSink, &QVideoSink::videoFrameChanged, converter, &VideoFrameConverter::processVideoFrame);
    if(newVideoSink != nullptr)
        connect(newVideoSink, &QVideoSink::videoFrameChanged, converter, &VideoFrameConverter::processVideoFrame);
    videoSink = newVideoSink;
    emit videoSinkChanged();
}

#ifndef QRCODESCANNER_H
#define QRCODESCANNER_H

#include "QRCodeReader.h"

class QRCodeScanner : public QRCodeReader
{
    Q_OBJECT
public:
    explicit QRCodeScanner(QObject *parent = nullptr);
    QVideoSink *getVideoSink() const;
    void setVideoSink(QVideoSink *newVideoSink);
public slots:
    void scan();
protected:
    QVideoSink *videoSink = nullptr;
signals:
    void videoSinkChanged();
private:
    Q_PROPERTY(QVideoSink *videoSink READ getVideoSink WRITE setVideoSink NOTIFY videoSinkChanged FINAL)
};

#endif // QRCODESCANNER_H

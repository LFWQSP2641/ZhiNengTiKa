#ifndef QRCODESCANNER_H
#define QRCODESCANNER_H

#include "QRCodeReader.h"

class QRCodeScanner : public QThread
{
    Q_OBJECT
public:
    explicit QRCodeScanner(QObject *parent = nullptr);
    ~QRCodeScanner();
    QVideoSink *getVideoSink() const;
    void setVideoSink(QVideoSink *newVideoSink);
    QRCodeReader *getQrCodeReader() const;
    bool getAutoStopOnSuccess() const;
    void setAutoStopOnSuccess(bool newAutoStopOnSuccess);

public slots:
    void startThread(QThread::Priority priority = InheritPriority);
    void stopThread();
    void waitForThreadFinish();

    void pauseScanning();
    void startScanning();
protected:
    QRCodeReader *qrCodeReader = nullptr;
    QVideoSink *videoSink = nullptr;

    bool autoStopOnSuccess = true;

    volatile bool canRun = false;
    volatile bool pause = false;
    void run() override;
signals:
    void decodingFinished(bool success, const ZXingResult &result);
    void error(const QString &msg);
    void videoSinkChanged();
    void autoStopOnSuccessChanged();

private:
    Q_PROPERTY(QVideoSink *videoSink READ getVideoSink WRITE setVideoSink NOTIFY videoSinkChanged FINAL)
    Q_PROPERTY(QRCodeReader *qrCodeReader READ getQrCodeReader CONSTANT FINAL)
    Q_PROPERTY(bool autoStopOnSuccess READ getAutoStopOnSuccess WRITE setAutoStopOnSuccess NOTIFY autoStopOnSuccessChanged FINAL)
};

#endif // QRCODESCANNER_H

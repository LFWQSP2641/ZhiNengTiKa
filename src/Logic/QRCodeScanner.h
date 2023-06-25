#ifndef QRCODESCANNER_H
#define QRCODESCANNER_H

#include <QObject>

class QRCodeScanner : public QObject
{
    Q_OBJECT
public:
    explicit QRCodeScanner(QObject *parent = nullptr);

    QVideoSink *getVideoSink() const;
    void setVideoSink(QVideoSink *newVideoSink);

    int getInterval() const;
    void setInterval(int newInterval);

    bool getContinueScan() const;

    bool getFinish() const;

    bool getScanning() const;

public slots:
    void scanQRCode(const QImage &image);
    void setScanning(bool newScanning);
    void startScanning();
    void stotScanning();
    void resetScanning();

protected:
    QVideoSink *videoSink = nullptr;
    bool finish = true;
    int interval = 1000;
    QTimer *intervalTimer;
    bool continueScan = false;
    bool scanning = false;

    bool initializeApi();

protected slots:
    void readVideoFrame(const QVideoFrame &frame);

signals:
    void apiInitializing();
    void initializeApiFinished(bool success);
    // success: 扫码是否有结果
    // exception: 扫码过程是否有异常 (exception为真时, success必为假, 不要有例外
    // decodeResult: 结果或异常信息
    void analysisFinished(bool success, bool exception, const QString &decodeResult);
    void videoSinkChanged();

    void intervalChanged();

    void scanningChanged();

private:
    Q_PROPERTY(QVideoSink *videoSink READ getVideoSink WRITE setVideoSink NOTIFY videoSinkChanged)
    Q_PROPERTY(int interval READ getInterval WRITE setInterval NOTIFY intervalChanged)
    Q_PROPERTY(bool continueScan READ getContinueScan CONSTANT)
    Q_PROPERTY(bool finish READ getFinish CONSTANT)
    Q_PROPERTY(bool scanning READ getScanning WRITE setScanning RESET resetScanning NOTIFY scanningChanged)
};

#endif // QRCODESCANNER_H

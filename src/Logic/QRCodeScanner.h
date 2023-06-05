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

public slots:
    void scanQRCode(const QImage &image);

protected:
    QVideoSink *videoSink = nullptr;
    bool finish = true;
    int interval = 1000;
    QTimer *intervalTimer;
    bool continueScan = false;

    bool initializeApi();

protected slots:
    void readVideoFrame(const QVideoFrame &frame);

signals:
    void apiInitializing();
    void initializeApiFinished(bool success);
    void analysisFinished(const QString &decodeResult);
    void videoSinkChanged();

    void intervalChanged();

private:
    Q_PROPERTY(QVideoSink *videoSink READ getVideoSink WRITE setVideoSink NOTIFY videoSinkChanged)
    Q_PROPERTY(int interval READ getInterval WRITE setInterval NOTIFY intervalChanged)
    Q_PROPERTY(bool continueScan READ getContinueScan CONSTANT)
    Q_PROPERTY(bool finish READ getFinish CONSTANT)
};

#endif // QRCODESCANNER_H

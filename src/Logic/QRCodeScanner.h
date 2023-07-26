#ifndef QRCODESCANNER_H
#define QRCODESCANNER_H

class QRCodeReader;

class VideoFrameConverter : public QObject
{
    Q_OBJECT
public:
    explicit VideoFrameConverter(QObject *parent = nullptr)
        : QObject{parent} {};

public slots:
    void processVideoFrame(const QVideoFrame &frame)
    {
        if (frame.isValid())
        {
            QImage image = frame.toImage();
            emit videoFrameChanged(image);
        }
    }

signals:
    void videoFrameChanged(const QImage &image);
};

class QRCodeScanner : public QObject
{
    Q_OBJECT
public:
    explicit QRCodeScanner(QObject *parent = nullptr);
    ~QRCodeScanner();
    QVideoSink *getVideoSink() const;
    void setVideoSink(QVideoSink *newVideoSink);

protected:
    QRCodeReader *reader;
    VideoFrameConverter *converter;
    QThread *thread;

    QVideoSink *videoSink = nullptr;
signals:
    void decodingFinished(bool succeeded, const QString &data);
    void error(const QString &msg);
    void videoSinkChanged();
private:
    Q_PROPERTY(QVideoSink *videoSink READ getVideoSink WRITE setVideoSink NOTIFY videoSinkChanged FINAL)
};

#endif // QRCODESCANNER_H

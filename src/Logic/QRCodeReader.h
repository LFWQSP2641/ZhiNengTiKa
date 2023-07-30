#ifndef QRCODEREADER_H
#define QRCODEREADER_H

#include "QRScanLib.h"

class QRCodeReader : public QObject
{
    Q_OBJECT
public:
    explicit QRCodeReader(QObject *parent = nullptr);

    int getMaxWidth() const;
    void setMaxWidth(int newMaxWidth);
    void resetMaxWidth();

    int getMaxHeight() const;
    void setMaxHeight(int newMaxHeight);
    void resetMaxHeight();

    bool getSmoothTransformation() const;
    void setSmoothTransformation(bool newSmoothTransformation);
    void resetSmoothTransformation();

public slots:
    QString decodeImageByPath(const QUrl &imagePath);
    QString decodeImage(const QImage &image);
    QString decodeFrame(const QVideoFrame &frame);

protected:
    QRScanLib scanner;

    int maxWidth = -1;
    int maxHeight = -1;
    bool smoothTransformation = false;

signals:
    void decodingFinished(bool succeeded, const QString &data);
    void error(const QString &msg);
    void maxWidthChanged();
    void maxHeightChanged();

    void smoothTransformationChanged();

private:
    Q_PROPERTY(int maxWidth READ getMaxWidth WRITE setMaxWidth RESET resetMaxWidth NOTIFY maxWidthChanged FINAL)
    Q_PROPERTY(int maxHeight READ getMaxHeight WRITE setMaxHeight RESET resetMaxHeight NOTIFY maxHeightChanged FINAL)
    Q_PROPERTY(bool smoothTransformation READ getSmoothTransformation WRITE setSmoothTransformation RESET resetSmoothTransformation NOTIFY smoothTransformationChanged FINAL)
};

#endif // QRCODEREADER_H

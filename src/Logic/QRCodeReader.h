#ifndef QRCODEREADER_H
#define QRCODEREADER_H

#include "LibZXingCpp/ZXingResult.h"

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
    ZXingResult *decodeImageByPath(const QUrl &imagePath);
    ZXingResult *decodeImage(const QImage &image);
    ZXingResult *decodeFrame(const QVideoFrame &frame);

protected:
    int maxWidth = -1;
    int maxHeight = -1;
    bool smoothTransformation = false;

    void warning(const QString &errMsg);
    void frameErrorWarning(const QString &errMsg);

signals:
    void decodingFinished(bool succeeded, ZXingResult *result);
    void error(const QString &msg);
    void frameError(const QString &msg);
    void maxWidthChanged();
    void maxHeightChanged();

    void smoothTransformationChanged();

private:
    Q_PROPERTY(int maxWidth READ getMaxWidth WRITE setMaxWidth RESET resetMaxWidth NOTIFY maxWidthChanged FINAL)
    Q_PROPERTY(int maxHeight READ getMaxHeight WRITE setMaxHeight RESET resetMaxHeight NOTIFY maxHeightChanged FINAL)
    Q_PROPERTY(bool smoothTransformation READ getSmoothTransformation WRITE setSmoothTransformation RESET resetSmoothTransformation NOTIFY smoothTransformationChanged FINAL)
};

#endif // QRCODEREADER_H

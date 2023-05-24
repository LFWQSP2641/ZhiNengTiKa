#ifndef QRCODESCANNER_H
#define QRCODESCANNER_H

#include <QObject>

class QRCodeScanner : public QObject
{
    Q_OBJECT
public:
    explicit QRCodeScanner(QObject *parent = nullptr);

public slots:
    void scanQRCode(const QImage &image);

protected:
    bool initializeApi();

signals:
    void apiInitializing();
    void initializeApiFinished(bool success);
    void analysisFinished(const QString &decodeResult);
};

#endif // QRCODESCANNER_H

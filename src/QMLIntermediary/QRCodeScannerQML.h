#ifndef QRCODESCANNERQML_H
#define QRCODESCANNERQML_H

class QRCodeScanner;

class QRCodeScannerQML: public QObject
{
    Q_OBJECT
public:
    explicit QRCodeScannerQML(QObject *parent = nullptr);
public slots:
    void readImage(const QVariant &imageVariant);
protected:
    QRCodeScanner *qrCodeScanner;
signals:
    void apiInitializing();
    void initializeApiFinished(bool success);
    void analysisFinished(const QString &decodeResult, bool success);
};

#endif // QRCODESCANNERQML_H

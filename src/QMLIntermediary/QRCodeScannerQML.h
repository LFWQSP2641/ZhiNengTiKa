#ifndef QRCODESCANNERQML_H
#define QRCODESCANNERQML_H

#include "../Logic/QRCodeScanner.h"

class QRCodeScannerQML: public QRCodeScanner
{
    Q_OBJECT
public:
    explicit QRCodeScannerQML(QObject *parent = nullptr);
public slots:
    void scanQRCodeFromFile(const QString &filePath);
signals:
};

#endif // QRCODESCANNERQML_H

#ifndef QRCODESCANNERQML_H
#define QRCODESCANNERQML_H

#include "../Logic/QRCodeScanner.h"

class QRCodeScannerQML: public QRCodeScanner
{
    Q_OBJECT
public:
    explicit QRCodeScannerQML(QObject *parent = nullptr);
public slots:
protected:
signals:
    void analysisFinishedQML(const QString &decodeResult, bool success);
};

#endif // QRCODESCANNERQML_H

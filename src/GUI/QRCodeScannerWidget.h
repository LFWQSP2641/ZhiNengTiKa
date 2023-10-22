#ifndef QRCODESCANNERWIDGET_H
#define QRCODESCANNERWIDGET_H

class QRCodeScanner;
class ZXingResult;

class QRCodeScannerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QRCodeScannerWidget(QWidget *parent = nullptr);
    ~QRCodeScannerWidget();

protected:
    QVBoxLayout *mainLayout;
    QCamera *camera;
    QMediaCaptureSession *mediaCaptureSession;
    QVideoWidget *videoWidget;
    QRCodeScanner *scanner;
    QPushButton *selectingFileButton;

protected slots:
    void onSelectingFileButtonPushed();

signals:
    void scanningFinished(bool succeeded, ZXingResult *result);
};

#endif // QRCODESCANNERWIDGET_H

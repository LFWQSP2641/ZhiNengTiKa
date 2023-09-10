#ifndef QRCODESCANNERWIDGET_H
#define QRCODESCANNERWIDGET_H

class QRCodeScanner;

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
    QPushButton *scanningButton;
    QPushButton *selectingFileButton;

protected slots:
    void onScanningButtonPushed();
    void onSelectingFileButtonPushed();

signals:
    void scanningFinished(bool success, const QString &result);
};

#endif // QRCODESCANNERWIDGET_H

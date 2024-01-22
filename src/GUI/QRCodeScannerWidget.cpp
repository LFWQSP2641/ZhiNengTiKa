#include "QRCodeScannerWidget.h"
#include "../Logic/QRCodeScanner.h"

QRCodeScannerWidget::QRCodeScannerWidget(QWidget *parent)
    : QWidget{parent},
      mainLayout(new QVBoxLayout(this)),
      camera(new QCamera(QMediaDevices::defaultVideoInput(), this)),
      mediaCaptureSession(new QMediaCaptureSession(this)),
      videoWidget(new QVideoWidget(this)),
      scanner(new QRCodeScanner(this)),
      selectingFileButton(new QPushButton(QStringLiteral("选择文件"), this))
{
    mediaCaptureSession->setCamera(camera);
    mediaCaptureSession->setVideoOutput(videoWidget);
    scanner->setVideoSink(videoWidget->videoSink());
    mainLayout->addWidget(videoWidget);
    auto tempLayout(new QHBoxLayout);
    tempLayout->addWidget(selectingFileButton);
    mainLayout->addLayout(tempLayout);

    connect(selectingFileButton, &QPushButton::clicked, this, &QRCodeScannerWidget::onSelectingFileButtonPushed);
    connect(scanner, &QRCodeScanner::decodingFinished, this, &QRCodeScannerWidget::scanningFinished);
    connect(scanner, &QRCodeScanner::error, [this](const QString & msg)
    {
        QMessageBox::warning(this, QStringLiteral("扫码失败"), msg);
    });

    camera->start();
    scanner->startThread();
}

QRCodeScannerWidget::~QRCodeScannerWidget()
{
    camera->stop();
    scanner->stopThread();
}

void QRCodeScannerWidget::onSelectingFileButtonPushed()
{
    const auto imageFilePath{QFileDialog::getOpenFileName(this, QStringLiteral("选择图片文件"), {}, QStringLiteral("Images (*.bmp *.gif *.jpg *.jpeg *.png *.tiff *.pbm *.pgm *.ppm *.xbm *.xpm)"))};
    if(imageFilePath.isEmpty())
        return;
    if(scanner->getQrCodeReader()->decodeImage(QImage(imageFilePath)).getText().isEmpty())
        QMessageBox::information(this, QStringLiteral("扫码失败"), QStringLiteral("扫码失败, 请尝试使用更清晰的图片"));
}

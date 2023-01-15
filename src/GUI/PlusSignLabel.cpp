#include "PlusSignLabel.h"
#ifdef Q_OS_ANDROID
#include "../StaticClass/CallAndroidNativeComponent.h"
#endif // Q_OS_ANDROID

PlusSignLabel::PlusSignLabel(QWidget *parent)
    : ClickableLabel{parent},
      menu(new QMenu(this)),
      pasteAction(new QAction(QStringLiteral("粘贴"), this)),
      addAction(new QAction(QStringLiteral("添加"), this))
{
    auto font{QFont()};
    font.setPixelSize(50);
    this->setFont(font);
    this->setFixedSize(50, 50);
    this->setText("+");

    menu->addAction(pasteAction);
    menu->addAction(addAction);

    connect(addAction, &QAction::triggered, this, &PlusSignLabel::selectImageFile);
    connect(pasteAction, &QAction::triggered, this, &PlusSignLabel::paste);
#ifdef Q_OS_ANDROID
    connect(this, &PlusSignLabel::clicked, this, &PlusSignLabel::showAskDialog);
#else // Q_OS_ANDROID
    connect(this, &PlusSignLabel::clicked, this, &PlusSignLabel::selectImageFile);
#endif // Q_OS_ANDROID

}

void PlusSignLabel::contextMenuEvent(QContextMenuEvent *event)
{
    this->menu->exec(event->globalPos());
}

void PlusSignLabel::selectImageFile()
{
    auto urlList{QFileDialog::getOpenFileUrls(this, QStringLiteral("选择文件"), QString(), QStringLiteral("Images (*.bmp *.gif *.jpg *.jpeg *.png *.tiff *.pbm *.pgm *.ppm *.xbm *.xpm)"))};
    if(urlList.isEmpty())
    {
        qDebug() << "PlusSignLabel::selectImageFile() : urlList.isEmpty()";
        return;
    }
    emit addPixmapLabels(urlList);
}

#ifdef Q_OS_ANDROID
void PlusSignLabel::showAskDialog()
{
    QDialog dialog;
    QHBoxLayout layout(&dialog);
    QPushButton takePhotoButton(QStringLiteral("拍照"), &dialog);
    QPushButton selectImageFileButton(QStringLiteral("选择文件"), &dialog);
    layout.addWidget(&takePhotoButton);
    layout.addWidget(&selectImageFileButton);
    connect(&takePhotoButton, &QPushButton::clicked, [this, &dialog]
    {
        auto image{CallAndroidNativeComponent::takePhoto()};
        if(!image.isNull())
        {
            emit addPixmapLabel(QPixmap::fromImage(image));
        }
        dialog.close();
    });
    connect(&selectImageFileButton, &QPushButton::clicked, [this, &dialog]
    {
        this->selectImageFile();
        dialog.close();
    });
    dialog.exec();
}
#endif // Q_OS_ANDROID

void PlusSignLabel::paste()
{
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    if (mimeData->hasImage())
    {
        emit addPixmapLabel(QPixmap::fromImage(qvariant_cast<QImage>(mimeData->imageData())));
    }
    else if(mimeData->hasUrls())
    {
        auto urlList{mimeData->urls()};
        if(urlList.isEmpty())
        {
            return;
        }
        emit addPixmapLabels(urlList);
    }
}

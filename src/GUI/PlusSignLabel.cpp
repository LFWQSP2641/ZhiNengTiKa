#include "PlusSignLabel.h"

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
    connect(this, &PlusSignLabel::clicked, this, &PlusSignLabel::selectImageFile);
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

#include "PixmapLabel.h"
#include "../StaticClass/XinjiaoyuNetwork.h"

PixmapLabel::PixmapLabel(QWidget *parent)
    : ClickableLabel{parent},
      menu(new QMenu(this)),
      copyAction(new QAction(QStringLiteral("复制"), this)),
      removeAction(new QAction(QStringLiteral("删除"), this)),
      previewAction(new QAction(QStringLiteral("预览"), this))
{
    this->setFixedSize(50, 50);
    this->setText("加载中");

    menu->addAction(copyAction);
    menu->addAction(removeAction);
    menu->addAction(previewAction);

    connect(copyAction, &QAction::triggered, this, &PixmapLabel::copyPixmap);
    connect(removeAction, &QAction::triggered, this, &PixmapLabel::removePixmap);
    connect(previewAction, &QAction::triggered, this, &PixmapLabel::previewPixmap);
    connect(this, &PixmapLabel::clicked, this, &PixmapLabel::showTipBox);
}

PixmapLabel::PixmapLabel(const QPixmap &pixmap, QWidget *parent)
    : PixmapLabel{parent}
{
    this->setPixmap(pixmap);
}

PixmapLabel::PixmapLabel(const QPixmap &pixmap, const QString &pixmapUrl, QWidget *parent)
    : PixmapLabel{parent}
{
    this->setPixmap(pixmap, pixmapUrl);
}

void PixmapLabel::setPixmap(const QPixmap &pixmap)
{
    this->uploadPixmap(pixmap);
}

void PixmapLabel::setPixmap(const QPixmap &pixmap, const QString &pixmapUrl)
{
    if(pixmapUrl.contains(QStringLiteral("xinjiaoyu.com")))
    {
        this->rawPixmap = pixmap;
        this->ClickableLabel::setPixmap(pixmap.scaled(this->size(), Qt::KeepAspectRatio));
        this->url = pixmapUrl;
    }
    else
    {
        this->setPixmap(pixmap);
    }
}

void PixmapLabel::uploadPixmap()
{
    uploadPixmap(this->rawPixmap);
}

void PixmapLabel::uploadPixmap(const QPixmap &pixmap)
{
    this->setEnabled(false);
    this->setText("上传中...");
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "JPG");
    auto reply{XinjiaoyuNetwork::uploadFileReply(bytes, QStringLiteral("image.jpg"))};
    connect(reply, &QNetworkReply::finished, [this, reply, pixmap]
    {
        QString infoStr;
        try
        {
            infoStr = XinjiaoyuNetwork::getUploadFileReplyUrl(reply) ;
        }
        catch(const std::exception &e)
        {
            QMessageBox::warning(this, QStringLiteral("warning"), e.what());
            remove();
            return;
        }
        this->url = infoStr;
        this->rawPixmap = pixmap;
        this->ClickableLabel::setPixmap(pixmap.scaled(this->size(), Qt::KeepAspectRatio));
        this->setEnabled(true);
    });
}

void PixmapLabel::contextMenuEvent(QContextMenuEvent *event)
{
    this->menu->exec(event->globalPos());
}

void PixmapLabel::copyPixmap()
{
    auto pixmap{this->rawPixmap};
    if(pixmap.isNull())
    {
        return;
    }
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setPixmap(pixmap);
}

void PixmapLabel::removePixmap()
{
    emit removed(this);
}

void PixmapLabel::previewPixmap()
{
    auto lable{new QLabel};
    lable->setAttribute(Qt::WA_DeleteOnClose);
    lable->setPixmap(this->rawPixmap);
    auto scrollArea{new QScrollArea};
    scrollArea->setAttribute(Qt::WA_DeleteOnClose);
    scrollArea->setWidget(lable);
    QScroller::grabGesture(scrollArea->viewport(), QScroller::TouchGesture);
    scrollArea->show();
}

void PixmapLabel::showTipBox()
{
    QMessageBox msgBox;
    QPushButton *ylButton = msgBox.addButton(QStringLiteral("预览"), QMessageBox::YesRole);
    QPushButton *deleteButton = msgBox.addButton(QStringLiteral("删除"), QMessageBox::DestructiveRole);
    QPushButton *cancelButton = msgBox.addButton(QStringLiteral("取消"), QMessageBox::NoRole);
    msgBox.setDefaultButton(cancelButton);
    msgBox.exec();

    if (msgBox.clickedButton() == deleteButton)
    {
        removePixmap();
    }
    else if (msgBox.clickedButton() == ylButton)
    {
        previewPixmap();
    }
}

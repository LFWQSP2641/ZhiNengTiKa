#include "PixmapLabel.h"
#include "../StaticClass/Network.h"
#include "../StaticClass/XinjiaoyuNetwork.h"

PixmapLabel::PixmapLabel(QWidget *parent)
    : QLabel(parent), pixmap(QPixmap(":/img/img/plus_sign.png"))
{
    this->setPixmap(pixmap.scaled(50, 50));
}

void PixmapLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if (event != nullptr && event->button() == Qt::LeftButton)
    {
        if(url.isEmpty())
        {
#ifdef Q_OS_ANDROID
            QtAndroidPrivate::requestPermission(QtAndroidPrivate::PermissionType::Storage);
#endif
            const auto fileName {QFileDialog::getOpenFileName(this, "getOpenFileName", ".", "Images (*.png *.jpg)")};
            if(!fileName.isEmpty())
            {
                this->setText("上传中...");
                pixmap = QPixmap(fileName);
                QByteArray bytes;
                QBuffer buffer(&bytes);
                buffer.open(QIODevice::WriteOnly);
                pixmap.save(&buffer, "JPG");
                auto reply{XinjiaoyuNetwork::uploadFileReply(bytes, QStringLiteral("image.jpg"))};
                connect(reply, &QNetworkReply::finished, [this, reply]
                {
                    auto fileUrl{ QJsonDocument::fromJson(Network::replyReadAll(reply)).object().value("data").toObject().value("accessUrl").toString() };
                    const auto lastSlashIndex{fileUrl.lastIndexOf("/")};
                    fileUrl.remove(lastSlashIndex + 1, fileUrl.size() - 1);
                    fileUrl.append(QByteArrayLiteral("image.jpg").toPercentEncoding());
                    url = fileUrl;
                    this->setPixmap(pixmap.scaled(50, 50));
                    emit pixmapSet();
                });
            }
        }
        else
        {
            QMessageBox msgBox;
            QPushButton *ylButton = msgBox.addButton(QStringLiteral("预览"), QMessageBox::YesRole);
            QPushButton *deleteButton = msgBox.addButton(QStringLiteral("删除"), QMessageBox::DestructiveRole);
            QPushButton *cancelButton = msgBox.addButton(QStringLiteral("取消"), QMessageBox::NoRole);
            msgBox.setDefaultButton(cancelButton);
            msgBox.exec();

            if (msgBox.clickedButton() == deleteButton)
            {
                url.clear();
                pixmap = QPixmap(":/img/img/plus_sign.png");
                this->setPixmap(pixmap.scaled(50, 50));
                emit pixmapRemoved(this);
            }
            else if (msgBox.clickedButton() == ylButton)
            {
                auto lable{new QLabel};
                lable->setAttribute(Qt::WA_DeleteOnClose);
                lable->setPixmap(this->pixmap);
                auto scrollArea{new QScrollArea};
                scrollArea->setAttribute(Qt::WA_DeleteOnClose);
                scrollArea->setWidget(lable);
                QScroller::grabGesture(scrollArea->viewport(), QScroller::TouchGesture);
#ifdef Q_OS_ANDROID
                scrollArea->showFullScreen();
#else
                scrollArea->show();
#endif
            }
        }
        event->accept();
    }
}

void PixmapLabel::setPixmapFromNetwork(const QString &url)
{
    if(url == "https://file.xinjiaoyu.com/files/image/no_answer.png")
    {
        return;
    }
    this->setText(QStringLiteral("获取中"));
    this->url = url;
    const auto reply{Network::networkAccessManager.get(QNetworkRequest(url))};
    connect(reply, &QNetworkReply::finished, [reply, this]
    {
        auto image{QImage::fromData(Network::replyReadAll(reply))};
        if(!image.isNull())
        {
            pixmap = QPixmap::fromImage(image);
        }
        else
        {
            pixmap = QPixmap(":/img/img/plus_sign.png");
        }
        this->setPixmap(pixmap.scaled(50, 50));
    });
    emit pixmapSet();
}

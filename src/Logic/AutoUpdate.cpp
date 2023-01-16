#include "AutoUpdate.h"
#include "../StaticClass/CallAndroidNativeComponent.h"
#include "../StaticClass/Network.h"

constexpr auto domain {"https://gitee.com/LFWQSP2641/xinjiaoyu/raw/master/"};

AutoUpdate::AutoUpdate(QObject *parent)
    : QObject{parent}
{

}

AutoUpdate::AutoUpdate(const QString &currentVersion, QObject *parent)
    : QObject{parent}, currentVersion{currentVersion}
{

}

void AutoUpdate::checkUpdate()
{
    running = true;
    hasNewVersion = false;
    newestVersionReply = nullptr;
    changeLogReply = nullptr;
    newestVersion = QString();
    changeLog = QString();

    newestVersionReply = Network::networkAccessManager->get(QNetworkRequest(QStringLiteral("newestVersion").prepend(domain)));
    QTimer::singleShot(5000, newestVersionReply, &QNetworkReply::abort);
    connect(newestVersionReply, &QNetworkReply::finished, this, &AutoUpdate::newestVersionReplyFinished);
}

bool AutoUpdate::checkMinimumVersion()
{
    auto minimumVersionStr{ Network::getData(QNetworkRequest(QStringLiteral("minimumVersion").prepend(domain))) };
    return compareVersion(minimumVersionStr, APP_VERSION);
}


void AutoUpdate::showUpdateWidget()
{
    if(!hasNewVersion)
    {
        return;
    }
    showUpdateWidgetPrivate();

}

void AutoUpdate::showResultWidget()
{
    if(hasNewVersion)
    {
        showUpdateWidget();
    }
    else
    {
        QDialog dialog;
        QGridLayout layout(&dialog);
        QLabel label(&dialog);
        QPushButton OKButton(QStringLiteral("确定"), &dialog);
        QPushButton stillUpdataButton(QStringLiteral("更新"), &dialog);

        if(newestVersion.isEmpty())
        {
            label.setText(QStringLiteral("当前版本:%0\n请检查网络连接,或使用移动网络再次尝试").arg(currentVersion));
        }
        else
        {
            label.setText(QStringLiteral("当前版本:%0\n已是最新版本").arg(currentVersion));
        }
        layout.addWidget(&label, 0, 0, 1, 2);
        layout.addWidget(&OKButton, 1, 0, 1, 1);
        layout.addWidget(&stillUpdataButton, 1, 1, 1, 1);

        QObject::connect(&OKButton, &QPushButton::clicked, [&dialog] { dialog.close(); });
        QObject::connect(&stillUpdataButton, &QPushButton::clicked, [this, &dialog]
        {
            dialog.close();
            changeLogReply = Network::networkAccessManager->get(QNetworkRequest(QStringLiteral("changeLog").prepend(domain)));
            Network::waitForFinished(changeLogReply);
            changeLog = Network::replyReadAll(changeLogReply);
            showUpdateWidgetPrivate();
        });

        dialog.exec();
    }
}

//如果version1比version2小,则返回true
bool AutoUpdate::compareVersion(const QString &version1, const QString &version2)
{
    QStringList list1 = version1.split(".");
    QStringList list2 = version2.split(".");
    if(list1.size() >= 3 && list2.size() >= 3)
    {
        qint32 ver1 = (list1.at(0).toInt() << 16) | (list1.at(1).toInt() << 8) | list1.at(2).toInt();
        qint32 ver2 = (list2.at(0).toInt() << 16) | (list2.at(1).toInt() << 8) | list2.at(2).toInt();
        return ver2 > ver1;
    }
    else
    {
        return false;
    }
}

void AutoUpdate::showUpdateWidgetPrivate()
{
    QMessageBox msgBox;
    msgBox.setText(QStringLiteral("当前版本:%0\n最新版本:%1\n\n%2\n\n是否下载?").arg(currentVersion, newestVersion, changeLog));
    auto downloadBtn{ msgBox.addButton(QStringLiteral("下载"), QMessageBox::AcceptRole) };
    auto openBrowerBtn{ msgBox.addButton(QStringLiteral("浏览器下载"), QMessageBox::AcceptRole) };
    msgBox.addButton(QStringLiteral("取消"), QMessageBox::RejectRole);
    msgBox.exec();

#ifdef Q_OS_ANDROID
    if(msgBox.clickedButton() == downloadBtn)
    {
        auto downloadUrlReply{Network::networkAccessManager->get(QNetworkRequest(QUrl(QStringLiteral("getNewestVersionEncryption").prepend(domain))))};
        auto progressDialog{new QDialog};
        auto progressLayout{new QVBoxLayout(progressDialog)};
        auto progressLabel{new QLabel(progressDialog)};
        progressLabel->setFixedHeight(progressLabel->fontMetrics().height());
        progressDialog->setAttribute(Qt::WA_DeleteOnClose);
        progressLayout->addWidget(progressLabel, Qt::AlignCenter);
        progressDialog->show();
        Network::waitForFinished(downloadUrlReply);
        const auto downloadUrl{Network::replyReadAll(downloadUrlReply)};
        qDebug() << "downloadUrl : " << downloadUrl;
        auto downloadReply{Network::networkAccessManager->get(QNetworkRequest(QUrl(downloadUrl)))};
        connect(downloadReply, &QNetworkReply::finished, [progressDialog, downloadReply]
        {
            if(!downloadReply->isFinished())
            {
                downloadReply->deleteLater();
                progressDialog->close();
                return;
            }
            auto fileData{Network::replyReadAll(downloadReply)};
            fileData[4] = static_cast<char>(120);
            fileData[5] = static_cast<char>(156);
            const QString filePath{CallAndroidNativeComponent::getCacheDir() + QDir::separator() + QStringLiteral("newVersion.apk")};
            QFile file(filePath);
            if(file.open(QFile::WriteOnly))
            {
                file.write(qUncompress(fileData));
                file.close();
                CallAndroidNativeComponent::installApk(filePath);
            }
            else
            {
                qWarning() << "AutoUpdate::showUpdateWidget() : !file.open(QFile::WriteOnly)";
                QMessageBox::warning(Q_NULLPTR, QStringLiteral("warning"), QStringLiteral("写入文件失败"));
            }
            progressDialog->close();
        });
        connect(downloadReply, &QNetworkReply::downloadProgress, [progressLabel](qint64 bytesReceived)
        {
            progressLabel->setText(QStringLiteral("已下载:%0").arg(bytesReceived));
            QApplication::processEvents();
        });
    }
    else if(msgBox.clickedButton() == openBrowerBtn)
    {
        auto reply{Network::networkAccessManager->get(QNetworkRequest(QUrl(QStringLiteral("getNewestVersion").prepend(domain))))};
        QDialog dialog;
        QVBoxLayout mianLayout(&dialog);

        QTextEdit showDownloadUrl(&dialog);
        QPushButton copyButton("复制", &dialog);

        QPushButton openBrowerButton(QStringLiteral("打开浏览器"), &dialog);
        QPushButton cancelButton(QStringLiteral("取消"), &dialog);

        QHBoxLayout tempLayout1;
        tempLayout1.addWidget(&showDownloadUrl);
        tempLayout1.addWidget(&copyButton);
        QHBoxLayout tempLayout2;
        tempLayout2.addWidget(&openBrowerButton);
        tempLayout2.addWidget(&cancelButton);

        mianLayout.addLayout(&tempLayout1);
        mianLayout.addLayout(&tempLayout2);

        Network::waitForFinished(reply);

        const QString downloadUrl(Network::replyReadAll(reply));
        qDebug() << "downloadUrl : " << downloadUrl;
        showDownloadUrl.setText(downloadUrl);

        connect(&copyButton, &QPushButton::clicked, [&downloadUrl]
        {
            QApplication::clipboard()->setText(downloadUrl);
            if(QApplication::clipboard()->text() == downloadUrl)
            {
                QMessageBox::information(Q_NULLPTR, QStringLiteral("information"), QStringLiteral("复制成功"));
            }
            else
            {
                QMessageBox::warning(Q_NULLPTR, QStringLiteral("warning"), QStringLiteral("复制失败"));
            }
        });
        connect(&openBrowerButton, &QPushButton::clicked, [&downloadUrl]
        {
            CallAndroidNativeComponent::openUrl(downloadUrl);
        });
        connect(&cancelButton, &QPushButton::clicked, [&dialog]
        {
            dialog.close();
        });
        dialog.exec();
    }
#endif // Q_OS_ANDROID
}

void AutoUpdate::newestVersionReplyFinished()
{
    newestVersion = Network::replyReadAll(newestVersionReply);
    //若无法连通,则newestVersion为空,导致compareVersion()为false
    if(compareVersion(currentVersion, newestVersion))
    {
        hasNewVersion = true;
        changeLogReply = Network::networkAccessManager->get(QNetworkRequest(QStringLiteral("changeLog").prepend(domain)));

        connect(changeLogReply, &QNetworkReply::finished, this, &AutoUpdate::changeLogReplyFinished);
    }
    else
    {
        //已是最新版本或newestVersion为空
        running = false;
        emit finished();
    }
}

void AutoUpdate::changeLogReplyFinished()
{
    changeLog = Network::replyReadAll(changeLogReply);
    running = false;
    emit finished();
}

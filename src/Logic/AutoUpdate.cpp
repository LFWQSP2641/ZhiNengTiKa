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
    qDebug() << "AutoUpdate::checkUpdate()";
    running = true;
    hasNewVersion = false;
    newestVersionReply = nullptr;
    downloadUrlReply = nullptr;
    changeLogReply = nullptr;
    newestVersion = QString();
    downloadUrl = QString();
    changeLog = QString();

    newestVersionReply = Network::networkAccessManager.get(QNetworkRequest(QStringLiteral("newestVersion").prepend(domain)));
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
    QDialog dialog;
    QGridLayout layout(&dialog);
    QLabel label(&dialog);
    QPushButton OKButton("确定", &dialog);
    QPushButton cancelButton("取消", &dialog);
    QTextEdit showDownloadUrl(&dialog);
    QPushButton copyButton("复制", &dialog);

    showDownloadUrl.setVisible(false);
    copyButton.setVisible(false);
    label.setWordWrap(true);
    label.setText(QStringLiteral("当前版本:%0\n最新版本:%1\n\n%2\n\n是否前往下载?").arg(currentVersion, newestVersion, changeLog));

    layout.addWidget(&label, 0, 0, 1, 2);
    layout.addWidget(&showDownloadUrl, 1, 0, 1, 1);
    layout.addWidget(&copyButton, 1, 1, 1, 1);
    layout.addWidget(&OKButton, 2, 0, 1, 1);
    layout.addWidget(&cancelButton, 2, 1, 1, 1);

    QObject::connect(&copyButton, &QPushButton::clicked, [this]
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

    QObject::connect(&OKButton, &QPushButton::clicked, [this, &label, &showDownloadUrl, &copyButton, &OKButton]
    {
        OKButton.setText(QStringLiteral("打开浏览器"));
#ifdef Q_OS_ANDROID
        CallAndroidNativeComponent::openUrl(downloadUrl);

        showDownloadUrl.setVisible(true);
        copyButton.setVisible(true);
        showDownloadUrl.setText(downloadUrl);
        label.setText(QStringLiteral("无法下载?\n请手动粘贴至浏览器下载"));

        //从Android 7.0开始的，不再允许在app中把file://Uri暴露给其他app，否则应用会抛出FileUriExposedException。
        //搞个FileProvider又挺麻烦...
        //所以直接传下载url

        //有空试试FileProvider吧

        /*
                QtAndroidPrivate::requestPermission(QtAndroidPrivate::Storage);

                const auto data{Global::replyReadAll(Global::requestAndWaitForFinished(QNetworkRequest(downloadUrl)))};

                qDebug() << data.size();
                qDebug() << data;

                const QString apkPath(Global::tempPath() + "/newVersion.apk");

                QFile newApk(apkPath);
                newApk.open(QFile::WriteOnly);
                newApk.write(data);
                newApk.close();

                //label->setText("下载完成");

                QJniObject jUrl = QJniObject::fromString(apkPath);
                QJniObject activity = QtAndroidPrivate::activity();

                QJniObject::callStaticMethod<void>(
                    "com/LFWQSP2641/xinjiaoyu/IntentActivity",
                    "installApk",
                    "(Ljava/lang/String;Lorg/qtproject/qt/android/bindings/QtActivity;)V",
                    jUrl.object<jstring>(),
                    activity.object<jobject>()
                );
                */

#endif // Q_OS_ANDROID
    });
    QObject::connect(&cancelButton, &QPushButton::clicked, [&dialog] { dialog.close(); });

    dialog.exec();
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
        QPushButton OKButton("确定", &dialog);

        if(newestVersion.isEmpty())
        {
            label.setText(QStringLiteral("当前版本:%0\n请检查网络连接,或使用移动网络再次尝试").arg(currentVersion));
        }
        else
        {
            label.setText(QStringLiteral("当前版本:%0\n已是最新版本").arg(currentVersion));
        }
        layout.addWidget(&label, 0, 0, 1, 1);
        layout.addWidget(&OKButton, 1, 0, 1, 1);

        QObject::connect(&OKButton, &QPushButton::clicked, [&dialog] { dialog.close(); });

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

void AutoUpdate::newestVersionReplyFinished()
{
    newestVersion = Network::replyReadAll(newestVersionReply);
    //若无法连通,则newestVersion为空,导致compareVersion()为false
    if(compareVersion(currentVersion, newestVersion))
    {
        hasNewVersion = true;
        downloadUrlReplyIsFinished = false;
        changeLogReplyIsFinished = false;
        downloadUrlReply = Network::networkAccessManager.get(QNetworkRequest(QStringLiteral("getNewestVersion").prepend(domain)));
        changeLogReply = Network::networkAccessManager.get(QNetworkRequest(QStringLiteral("changeLog").prepend(domain)));

        connect(downloadUrlReply, &QNetworkReply::finished, this, &AutoUpdate::downloadUrlReplyFinished);
        connect(changeLogReply, &QNetworkReply::finished, this, &AutoUpdate::changeLogReplyFinished);
    }
    else
    {
        //已是最新版本或newestVersion为空
        running = false;
        emit finished();
    }
}

void AutoUpdate::downloadUrlReplyFinished()
{
    downloadUrl = Network::replyReadAll(downloadUrlReply);
    downloadUrlReplyIsFinished = true;
    if(changeLogReplyIsFinished)
    {
        running = false;
        emit finished();
    }
}

void AutoUpdate::changeLogReplyFinished()
{
    changeLog = Network::replyReadAll(changeLogReply);
    changeLogReplyIsFinished = true;
    if(downloadUrlReplyIsFinished)
    {
        running = false;
        emit finished();
    }
}

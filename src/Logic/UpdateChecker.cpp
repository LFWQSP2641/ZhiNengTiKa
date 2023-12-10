#include "UpdateChecker.h"
#include "NetworkAccessManagerBlockable.h"
#include "../StaticClass/Global.h"
#include "../Singleton/Network.h"
#ifdef Q_OS_ANDROID
#include "../StaticClass/CallAndroidNativeComponent.h"
#endif // Q_OS_ANDROID

const Version UpdateChecker::currentVersion = Version(QStringLiteral(APP_VERSION));

UpdateChecker::UpdateChecker(QObject *parent)
    : QThread{parent}
{

}

bool UpdateChecker::checkMinimumVersion() const
{
    auto minimumVersionStr{ Network::Network::getGlobalNetworkManager()->getDataByStrUrl(QStringLiteral("Update/minimumVersion").prepend(DATABASE_DOMAIN)) };
    qDebug() << minimumVersionStr;
    return Version(minimumVersionStr) < UpdateChecker::currentVersion;
}

Version UpdateChecker::getCurrentVersion() const
{
    return currentVersion;
}

Version UpdateChecker::getNewestVersion() const
{
    return newestVersion;
}

QString UpdateChecker::getChangeLog() const
{
    return changeLog;
}

bool UpdateChecker::getRunning() const
{
    return running;
}

bool UpdateChecker::getHasNewVersion() const
{
    return hasNewVersion;
}

void UpdateChecker::downloadNewestVersion()
{
    QFile file(Global::tempPath().append(QStringLiteral("/newVersion.apk")));
    QFile fileVersion(Global::tempPath().append(QStringLiteral("/newVersion.txt")));
    if(file.size() > 0 && fileVersion.open(QFile::ReadOnly))
    {
        const QString version(fileVersion.readAll());
        fileVersion.close();
        if(Version(version) == newestVersion)
            emit this->downloadFinished();
        return;
    }
    auto newestVersionReply(Network::getGlobalNetworkManager()->getByStrUrl(Network::getGlobalNetworkManager()->getDataByStrUrl(QStringLiteral("Update/getNewestVersionEncryption").prepend(DATABASE_DOMAIN))));
    connect(newestVersionReply, &QNetworkReply::downloadProgress, this, &UpdateChecker::downloadProgress);
    connect(newestVersionReply, &QNetworkReply::finished, [newestVersionReply, this]
    {
        auto fileData(newestVersionReply->readAll());
        newestVersionReply->deleteLater();
        fileData[4] = static_cast<char>(120);
        fileData[5] = static_cast<char>(156);
        QFile file(Global::tempPath().append(QStringLiteral("/newVersion.apk")));
        file.open(QFile::WriteOnly);
        file.write(qUncompress(fileData));
        file.close();

        QFile fileVersion(Global::tempPath().append(QStringLiteral("/newVersion.txt")));
        fileVersion.open(QFile::WriteOnly);
        fileVersion.write(newestVersion.toString().toUtf8());
        fileVersion.close();

        emit this->downloadFinished();
    });
}

void UpdateChecker::installNewestVersion()
{
#ifdef Q_OS_ANDROID
    CallAndroidNativeComponent::installApk(Global::tempPath().append(QStringLiteral("/newVersion.apk")));
#endif // Q_OS_ANDROID
}

void UpdateChecker::run()
{
    this->running = true;
    this->hasNewVersion = false;

    NetworkAccessManagerBlockable networkAccessManagerBlockable;

    auto newestVersionReply{networkAccessManagerBlockable.getByStrUrl(QStringLiteral("Update/newestVersion").prepend(DATABASE_DOMAIN))};
    auto changeLogReply{networkAccessManagerBlockable.getByStrUrl(QStringLiteral("Update/changeLog").prepend(DATABASE_DOMAIN))};

    this->newestVersion = Version(networkAccessManagerBlockable.replyReadAll(networkAccessManagerBlockable.replyWaitForFinished(newestVersionReply)));
    this->changeLog = networkAccessManagerBlockable.replyReadAll(networkAccessManagerBlockable.replyWaitForFinished(changeLogReply));

    this->hasNewVersion = (this->currentVersion < this->newestVersion);

    QFile file(Global::tempPath().append(QStringLiteral("/newVersion.apk")));
    QFile fileVersion(Global::tempPath().append(QStringLiteral("/newVersion.txt")));
    if(file.size() > 0 && fileVersion.open(QFile::ReadOnly))
    {
        const QString version(fileVersion.readAll());
        fileVersion.close();
        if(Version(version) == currentVersion)
        {
            file.remove();
            fileVersion.remove();
        }
    }

    this->running = false;

    qDebug() << Q_FUNC_INFO << "检查完成" << hasNewVersion;

    emit checkFinished(this->hasNewVersion);
    if(this->hasNewVersion)
        emit checkFinishedAndHasNewVersion();
}


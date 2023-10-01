#include "UpdateChecker.h"
#include "NetworkAccessManagerBlockable.h"
#include "../StaticClass/Global.h"
#include "../Singleton/Network.h"
#ifdef Q_OS_ANDROID
#include "../StaticClass/CallAndroidNativeComponent.h"
#endif // Q_OS_ANDROID

const QString UpdateChecker::currentVersion = QStringLiteral(APP_VERSION);

UpdateChecker::UpdateChecker(QObject *parent)
    : QThread{parent}
{

}

int UpdateChecker::compareVersion(const QString &version1, const QString &version2)
{
    QStringList list1 = version1.split(".");
    QStringList list2 = version2.split(".");
    if(list1.size() >= 3 && list2.size() >= 3)
    {
        qint32 ver1 = (list1.at(0).toInt() << 16) | (list1.at(1).toInt() << 8) | list1.at(2).toInt();
        qint32 ver2 = (list2.at(0).toInt() << 16) | (list2.at(1).toInt() << 8) | list2.at(2).toInt();
        if(ver1 > ver2)
        {
            return 1;
        }
        else if(ver1 < ver2)
        {
            return -1;
        }
    }
    return 0;
}

bool UpdateChecker::checkMinimumVersion() const
{
    auto minimumVersionStr{ Network::Network::getGlobalNetworkManager()->getDataByStrUrl(QStringLiteral("Update/minimumVersion").prepend(DATABASE_DOMAIN)) };
    qDebug() << minimumVersionStr;
    return compareVersion(minimumVersionStr, UpdateChecker::currentVersion) < 0;
}

QString UpdateChecker::getCurrentVersion() const
{
    return currentVersion;
}

QString UpdateChecker::getNewestVersion() const
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

    this->newestVersion = networkAccessManagerBlockable.replyReadAll(networkAccessManagerBlockable.replyWaitForFinished(newestVersionReply));
    this->changeLog = networkAccessManagerBlockable.replyReadAll(networkAccessManagerBlockable.replyWaitForFinished(changeLogReply));

    this->hasNewVersion = UpdateChecker::compareVersion(this->currentVersion, this->newestVersion) < 0;

    emit checkFinished(this->hasNewVersion);
}


#include "LibraryUpdateChecker.h"
#include "NetworkAccessManagerBlockable.h"
#include "../StaticClass/Global.h"
#include "../Singleton/Network.h"

LibraryUpdateChecker::LibraryUpdateChecker(QObject *parent)
    : LibraryUpdateChecker{QStringLiteral("0.0.0"), parent}
{

}

LibraryUpdateChecker::LibraryUpdateChecker(const QString &currentVersion, QObject *parent)
    : QThread{parent}, currentVersion(currentVersion)
{

}

Version LibraryUpdateChecker::getCurrentVersion() const
{
    return currentVersion;
}

Version LibraryUpdateChecker::getNewestVersion() const
{
    return newestVersion;
}

QString LibraryUpdateChecker::getChangeLog() const
{
    return changeLog;
}

bool LibraryUpdateChecker::getRunning() const
{
    return running;
}

bool LibraryUpdateChecker::getHasNewVersion() const
{
    return hasNewVersion;
}

void LibraryUpdateChecker::downloadNewestVersion()
{
    qDebug() << Q_FUNC_INFO;
    auto newestVersionReply(Network::getGlobalNetworkManager()->getByStrUrl(Network::getGlobalNetworkManager()->getDataByStrUrl(QStringLiteral("LibraryUpdate/getNewestVersionEncryption").prepend(DATABASE_DOMAIN))));
    connect(newestVersionReply, &QNetworkReply::downloadProgress, this, &LibraryUpdateChecker::downloadProgress);
    connect(newestVersionReply, &QNetworkReply::finished, [newestVersionReply, this]
    {
        auto fileData(newestVersionReply->readAll());
        newestVersionReply->deleteLater();
        fileData[4] = static_cast<char>(120);
        fileData[5] = static_cast<char>(156);
        QFile file(Global::dataPath().append(QStringLiteral("/newVersionLibrary")));
        file.open(QFile::WriteOnly);
        file.write(qUncompress(fileData));
        file.close();

        emit this->downloadFinished();
    });
}

void LibraryUpdateChecker::setCurrentVersion(const Version &newCurrentVersion)
{
    currentVersion = newCurrentVersion;
}

void LibraryUpdateChecker::run()
{
    this->running = true;
    this->hasNewVersion = false;

    NetworkAccessManagerBlockable networkAccessManagerBlockable;

    auto newestVersionReply{networkAccessManagerBlockable.getByStrUrl(QStringLiteral("LibraryUpdate/newestVersion").prepend(DATABASE_DOMAIN))};
    auto changeLogReply{networkAccessManagerBlockable.getByStrUrl(QStringLiteral("LibraryUpdate/changeLog").prepend(DATABASE_DOMAIN))};

    this->newestVersion = Version(networkAccessManagerBlockable.replyReadAll(networkAccessManagerBlockable.replyWaitForFinished(newestVersionReply)));
    this->changeLog = networkAccessManagerBlockable.replyReadAll(networkAccessManagerBlockable.replyWaitForFinished(changeLogReply));

    this->hasNewVersion = (this->currentVersion < this->newestVersion);

    this->running = false;

    qDebug() << Q_FUNC_INFO << "检查完成" << hasNewVersion;

    emit checkFinished(this->hasNewVersion);
    if(this->hasNewVersion)
        emit checkFinishedAndHasNewVersion();
}

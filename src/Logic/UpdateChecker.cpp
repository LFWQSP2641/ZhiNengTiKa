#include "UpdateChecker.h"
#include "../Singleton/Network.h"

UpdateChecker::UpdateChecker(QObject *parent)
    : UpdateChecker{Version(), parent}
{
}

UpdateChecker::UpdateChecker(const Version &currentVersion, QObject *parent)
    : QObject{parent},
      currentVersion(currentVersion)
{
}

bool UpdateChecker::checkMinimumVersion() const
{
    auto minimumVersionStr{ Network::Network::getGlobalNetworkManager()->getDataByStrUrl(getDomain().append(QStringLiteral("minimumVersion"))) };
    qDebug() << minimumVersionStr;
    if(minimumVersionStr.isEmpty())
        return false;
    return Version(minimumVersionStr) < UpdateChecker::currentVersion;
}

Version UpdateChecker::getCurrentVersion() const
{
    return currentVersion;
}

void UpdateChecker::setCurrentVersion(const Version &newCurrentVersion)
{
    currentVersion = newCurrentVersion;
}

Version UpdateChecker::getNewestVersion() const
{
    return newestVersion;
}

QString UpdateChecker::getChangeLog() const
{
    return changeLog;
}

bool UpdateChecker::isRunning() const
{
    return running;
}

bool UpdateChecker::isFinished() const
{
    return !running;
}

bool UpdateChecker::getHasNewVersion() const
{
    return hasNewVersion;
}

void UpdateChecker::start()
{
    this->running = true;
    this->hasNewVersion = false;
    this->finishedFlag.fill(false);

    auto newestVersionReply{Network::getGlobalNetworkManager()->getByStrUrl(getDomain().append(QStringLiteral("newestVersion")))};
    auto changeLogReply{Network::getGlobalNetworkManager()->getByStrUrl(getDomain().append(QStringLiteral("changeLog")))};

    connect(newestVersionReply, &QNetworkReply::finished, this, &UpdateChecker::onNewestVersionReplyFinished);
    connect(changeLogReply, &QNetworkReply::finished, this, &UpdateChecker::onChangeLogReplyFinished);
}

void UpdateChecker::downloadNewestVersion()
{
    auto downloadUrlRelpy(Network::getGlobalNetworkManager()->getByStrUrl(getDomain().append(QStringLiteral("getNewestVersionEncryption"))));
    connect(downloadUrlRelpy, &QNetworkReply::finished, this, &UpdateChecker::onFetchDownloadUrlRelpyFinished);
    // auto newestVersionReply(Network::getGlobalNetworkManager()->getByStrUrl(Network::getGlobalNetworkManager()->getDataByStrUrl(getDomain().append(QStringLiteral("getNewestVersionEncryption")))));
    // connect(newestVersionReply, &QNetworkReply::downloadProgress, this, &UpdateChecker::downloadProgress);
    // connect(newestVersionReply, &QNetworkReply::finished, [newestVersionReply, this]
    // {
    //     auto fileData(newestVersionReply->readAll());
    //     newestVersionReply->deleteLater();
    //     fileData[4] = static_cast<char>(120);
    //     fileData[5] = static_cast<char>(156);
    //     QFile file(getSaveFilePath());
    //     file.open(QFile::WriteOnly);
    //     file.write(qUncompress(fileData));
    //     file.close();

    //     emit this->downloadFinished();
    // });
}

void UpdateChecker::onNewestVersionReplyFinished()
{
    auto reply(qobject_cast<QNetworkReply*>(sender()));
    if(reply->error() == QNetworkReply::NoError)
    {
        this->newestVersion = Version(Network::getGlobalNetworkManager()->replyReadAll(reply));
        finishedFlag[0] = true;
        if(finishedFlag.count(true) == 2)
            taskEnding();
    }
    else
    {
        emit error(reply->errorString());
        reply->deleteLater();
        taskFailing();
    }
}

void UpdateChecker::onChangeLogReplyFinished()
{
    auto reply(qobject_cast<QNetworkReply*>(sender()));
    if(reply->error() == QNetworkReply::NoError)
    {
        this->changeLog = Network::getGlobalNetworkManager()->replyReadAll(reply);
        finishedFlag[1] = true;
        if(finishedFlag.count(true) == 2)
            taskEnding();
    }
    else
    {
        emit error(reply->errorString());
        reply->deleteLater();
        taskFailing();
    }
}

void UpdateChecker::taskEnding()
{
    this->hasNewVersion = (this->currentVersion < this->newestVersion);
    this->running = false;

    qDebug() << Q_FUNC_INFO << "检查完成" << hasNewVersion;

    emit checkFinished(this->hasNewVersion);
    if(this->hasNewVersion)
        emit checkFinishedAndHasNewVersion();
}

void UpdateChecker::taskFailing()
{
    this->running = false;
    this->hasNewVersion = false;
    this->finishedFlag.fill(false);
    emit failed();
}

void UpdateChecker::onFetchDownloadUrlRelpyFinished()
{
    auto reply(qobject_cast<QNetworkReply*>(sender()));
    if(reply->error() == QNetworkReply::NoError)
    {
        auto newestVersionReply(Network::getGlobalNetworkManager()->getByStrUrl(Network::getGlobalNetworkManager()->replyReadAll(reply)));
        connect(newestVersionReply, &QNetworkReply::finished, this, &UpdateChecker::onDownloadNewestVersionRelpyFinished);
    }
    else
    {
        emit error(reply->errorString());
        reply->deleteLater();
    }
}

void UpdateChecker::onDownloadNewestVersionRelpyFinished()
{
    auto reply(qobject_cast<QNetworkReply*>(sender()));
    if(reply->error() == QNetworkReply::NoError)
    {
        auto fileData(Network::getGlobalNetworkManager()->replyReadAll(reply));
        fileData[4] = static_cast<char>(120);
        fileData[5] = static_cast<char>(156);
        QFile file(getSaveFilePath());
        file.open(QFile::WriteOnly);
        file.write(qUncompress(fileData));
        file.close();

        emit this->downloadFinished();
    }
    else
    {
        emit error(reply->errorString());
        reply->deleteLater();
    }
}

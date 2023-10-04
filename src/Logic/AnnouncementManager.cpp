#include "AnnouncementManager.h"
#include "AnnouncementModel.h"
#include "../Singleton/Network.h"
#include "../StaticClass/Global.h"
#include "UpdateChecker.h"

AnnouncementManager::AnnouncementManager(QObject *parent)
    : QObject{parent},
      announcementModel(new AnnouncementModel(this))
{

}

void AnnouncementManager::obtainAnnouncement()
{
    auto reply(Network::getGlobalNetworkManager()->getByStrUrl(QStringLiteral("Announcement").prepend(DATABASE_DOMAIN)));
    connect(reply, &QNetworkReply::finished, this, [this, reply]
    {
        if(reply->error() != QNetworkReply::NoError)
        {
            emit error(reply->errorString());
            reply->deleteLater();
            return;
        }
        analysisRawData(reply->readAll());
        reply->deleteLater();
    });
}

void AnnouncementManager::markAnnouncementAsRead(int index)
{
    if(announcementModel->readList[index] == true)
        return;
    announcementModel->readList[index] = true;
    const auto title(announcementModel->titleList.at(index));
    const auto detail(announcementModel->detailList.at(index));
    QFile file(Global::configPath().append(QStringLiteral("/AnnouncementAsRead")));
    file.open(QFile::ReadWrite | QFile::Append);
    file.write(QCryptographicHash::hash((title + detail).toUtf8(), QCryptographicHash::Md5).toBase64());
    file.write(QByteArrayLiteral("\n"));
    file.close();
}

AnnouncementModel *AnnouncementManager::getAnnouncementModel() const
{
    return announcementModel;
}

void AnnouncementManager::analysisRawData(const QByteArray &data)
{
    QJsonParseError ok;
    const auto jsonDocument(QJsonDocument::fromJson(data, &ok));
    if(ok.error != QJsonParseError::NoError)
    {
        emit error(ok.errorString());
        return;
    }
    if(!jsonDocument.isArray())
    {
        emit error(QStringLiteral("源数据不为JsonArray"));
        return;
    }
    QStringList titleList;
    QStringList detailList;
    QList<bool> readList;
    QStringList announcementHashList;
    jsonArray = jsonDocument.array();
    for(const auto &i : qAsConst(jsonArray))
    {
        const auto jsonObject(i.toObject());
        const auto platform(jsonObject.value(QStringLiteral("platform")).toString().toLower());
        if(platform != QSysInfo::productType().toLower() && platform != QStringLiteral("all") && (!platform.isEmpty()))
        {
            continue;
        }
        const auto version(jsonObject.value(QStringLiteral("version")).toString().toLower());
        const auto result(UpdateChecker::compareVersion(QStringLiteral(APP_VERSION), version));
        const auto versionComparisonMethod(jsonObject.value(QStringLiteral("versionComparisonMethod")).toString().toLower());
        bool pass((result == 0 && versionComparisonMethod.contains(QStringLiteral("="))) ||
                  (result < 0 && versionComparisonMethod.contains(QStringLiteral("<"))) ||
                  (result > 0 && versionComparisonMethod.contains(QStringLiteral(">"))));
        if(!pass)
        {
            continue;
        }
        const auto title(jsonObject.value(QStringLiteral("title")).toString());
        const auto detail(jsonObject.value(QStringLiteral("detail")).toString());
        titleList.append(title);
        detailList.append(detail);
        announcementHashList.append(QCryptographicHash::hash((title + detail).toUtf8(), QCryptographicHash::Md5).toBase64());
    }

    QStringList localAnnouncementAsReadHashList;
    QFile file(Global::configPath().append(QStringLiteral("/AnnouncementAsRead")));
    if(file.size() > 0)
    {
        file.open(QFile::ReadOnly);
        while (!file.atEnd())
        {
            auto hash(file.readLine());
            if(hash.endsWith(QByteArrayLiteral("\n")))
                hash.resize(hash.size() - 1);
            localAnnouncementAsReadHashList.append(hash);
        }
        file.close();
    }

    QStringList announcementAsReadHashList;
    for(const auto &i : announcementHashList)
    {
        if(localAnnouncementAsReadHashList.contains(i))
        {
            readList.append(true);
            announcementAsReadHashList.append(i);
        }
        else
        {
            readList.append(false);
        }
    }

    file.open(QFile::WriteOnly);
    for(const auto &i : announcementAsReadHashList)
    {
        file.write(i.toUtf8());
        file.write(QByteArrayLiteral("\n"));
    }
    file.close();

    bool hasNew(false);

    for(const auto &i : readList)
    {
        if(!i)
            hasNew = true;
    }

    if(!announcementModel->titleList.isEmpty())
    {
        announcementModel->beginRemoveRows(QModelIndex(), 0, announcementModel->titleList.size() - 1);
        announcementModel->titleList.clear();
        announcementModel->detailList.clear();
        announcementModel->readList.clear();
        announcementModel->endRemoveRows();
    }
    announcementModel->beginInsertRows(QModelIndex(), 0, titleList.size() - 1);
    announcementModel->titleList = std::move(titleList);
    announcementModel->detailList = std::move(detailList);
    announcementModel->readList = std::move(readList);
    announcementModel->endInsertRows();

    emit obtainFinished(hasNew);
}

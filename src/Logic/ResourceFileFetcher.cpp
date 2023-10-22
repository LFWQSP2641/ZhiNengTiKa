#include "ResourceFileFetcher.h"
#include "../StaticClass/XinjiaoyuEncryptioner.h"
#include "../Singleton/Network.h"

ResourceFileFetcher::ResourceFileFetcher(QObject *parent)
    : QObject{parent},
      model(new ResourceFileModel(this))
{

}

void ResourceFileFetcher::init()
{
    auto reply(Network::getGlobalNetworkManager()->get(setRequest(QStringLiteral("https://www.xinjiaoyu.com/api/v3/server_resource/book/catalog/school/center?schoolId=100117"))));
    connect(reply, &QNetworkReply::finished, this, &ResourceFileFetcher::onCatalogArrayReplyFinished);
}

void ResourceFileFetcher::continueLoadModel()
{
    if(currentPage >= maxPage)
    {
        emit error(QStringLiteral("尝试加载页数超过最大值"));
        emit continueLoadModelFinished();
        return;
    }
    ++currentPage;
    auto reply(Network::getGlobalNetworkManager()->get(setRequest(QStringLiteral("https://www.xinjiaoyu.com/api/v3/server_resource/homework/resource?bookCatalogId=%0&current=%1&size=10").arg(currentBookCatalogId, QString::number(currentPage)))));
    connect(reply, &QNetworkReply::finished, this, &ResourceFileFetcher::onContinueLoadResourceReplyFinished);
}

void ResourceFileFetcher::resetModel(const QString &subject, const QString &edition, const QString &module)
{
    currentBookCatalogId.clear();
    currentPage = 1;
    maxPage = 1;
    const auto categoryAttributeTreeList(catalogDetail
                                         .value(QStringLiteral("data")).toObject()
                                         .value(QStringLiteral("categoryAttributeTreeList")).toArray());
    const auto categoryAttributeSubjectTrees(categoryAttributeTreeList
            .at(0).toObject()
            .value(QStringLiteral("categoryAttributeTrees")).toArray());
    QString subjectId;
    QString editionId;
    QString moduleId;
    // 什么嵌套天堂
    for(const auto &i : categoryAttributeSubjectTrees)
    {
        const auto subjectObject(i.toObject());
        if(subjectObject.value(QStringLiteral("attributeValue")).toString() == subject)
        {
            subjectId = subjectObject.value(QStringLiteral("attributeId")).toString();
            const auto categoryAttributeEditionTrees(subjectObject.value(QStringLiteral("categoryAttributeTrees")).toArray());
            for(const auto &j : categoryAttributeEditionTrees)
            {
                const auto editionObject(j.toObject());
                if(editionObject.value(QStringLiteral("attributeValue")).toString() == edition)
                {
                    editionId = editionObject.value(QStringLiteral("attributeId")).toString();
                    const auto categoryAttributeModuleTrees(editionObject.value(QStringLiteral("categoryAttributeTrees")).toArray());
                    for(const auto &k : categoryAttributeModuleTrees)
                    {
                        const auto moduleObject(k.toObject());
                        if(moduleObject.value(QStringLiteral("attributeValue")).toString() == module)
                        {
                            moduleId = moduleObject.value(QStringLiteral("attributeId")).toString();
                        }
                    }
                }
            }
        }
    }

    const auto categoryAttributeList(catalogDetail
                                     .value(QStringLiteral("data")).toObject()
                                     .value(QStringLiteral("categoryAttributeList")).toArray());
    QJsonArray bookCatalogDetailList;
    for(const auto &i : categoryAttributeList)
    {
        const auto object(i.toObject());
        QJsonObject pObject;
        pObject.insert(QStringLiteral("categoryAttributeId"), object.value(QStringLiteral("attributeNameId")).toString());
        const auto attributeName(object.value(QStringLiteral("attributeName")).toString());
        if(attributeName == QStringLiteral("学段"))
        {
            pObject.insert(QStringLiteral("attributeValueId"), categoryAttributeTreeList.at(0).toObject().value(QStringLiteral("attributeId")).toString());
        }
        else if(attributeName == QStringLiteral("学科"))
        {
            pObject.insert(QStringLiteral("attributeValueId"), subjectId);
        }
        else if(attributeName == QStringLiteral("版次"))
        {
            pObject.insert(QStringLiteral("attributeValueId"), editionId);
        }
        else if(attributeName == QStringLiteral("模块"))
        {
            pObject.insert(QStringLiteral("attributeValueId"), moduleId);
        }
        bookCatalogDetailList.append(pObject);
    }

    QJsonObject rootJsonObject;
    rootJsonObject.insert(QStringLiteral("categoryId"), categoryAttributeList.at(0).toObject().value(QStringLiteral("categoryId")).toString());
    rootJsonObject.insert(QStringLiteral("bookCatalogDetailList"), bookCatalogDetailList);

    const auto postData(QJsonDocument(rootJsonObject).toJson(QJsonDocument::Compact));

    auto reply(Network::getGlobalNetworkManager()->post(setRequest(QStringLiteral("https://www.xinjiaoyu.com/api/v3/server_resource/book/catalog")), postData));
    connect(reply, &QNetworkReply::finished, this, &ResourceFileFetcher::onCatalogReplyFinished);
}

QNetworkRequest ResourceFileFetcher::setRequest(const QUrl &url)
{
    QNetworkRequest requestInfo;
    requestInfo.setUrl(url);
    requestInfo.setRawHeader(QByteArrayLiteral("Content-Type"), QByteArrayLiteral("application/json"));
    requestInfo.setRawHeader(QByteArrayLiteral("client"), QByteArrayLiteral("front"));
    requestInfo.setRawHeader(QByteArrayLiteral("Authorization"), generateRandomString(976).toUtf8());
    requestInfo.setRawHeader(QByteArrayLiteral("accessToken"), generateRandomString(32).toUtf8());

    const auto tVal{QString::number(QDateTime::currentMSecsSinceEpoch()).toUtf8()};
    requestInfo.setRawHeader(QByteArrayLiteral("t"), tVal);

    const auto clientSession(generateRandomString(10).toUtf8().prepend(tVal));
    requestInfo.setRawHeader(QByteArrayLiteral("clientSession"), clientSession);
    const auto encryptVal{XinjiaoyuEncryptioner::getXinjiaoyuMD5(tVal, clientSession)};
    requestInfo.setRawHeader(QByteArrayLiteral("encrypt"), encryptVal);
    return requestInfo;
}

QString ResourceFileFetcher::generateRandomString(qsizetype size)
{
    QString randomString;
    for(auto i(0); i < size; ++i)
    {
        const auto randomInt{ QRandomGenerator::global()->bounded(62) };
        if(randomInt < 10)
        {
            randomString.append(QString::number(randomInt));
        }
        else if(randomInt < 36)
        {
            randomString.append(static_cast<char>(55 + randomInt));
        }
        else
        {
            randomString.append(static_cast<char>(61 + randomInt));
        }
    }
    return randomString;
}

ResourceFileModel *ResourceFileFetcher::getModel() const
{
    return model;
}

QVariant ResourceFileFetcher::getSubjectModel()
{
    QStringList list;
    const auto categoryAttributeSubjectTrees(catalogDetail
            .value(QStringLiteral("data")).toObject()
            .value(QStringLiteral("categoryAttributeTreeList")).toArray()
            .at(0).toObject()
            .value(QStringLiteral("categoryAttributeTrees")).toArray());
    for(const auto &i : categoryAttributeSubjectTrees)
    {
        list.append(i.toObject().value(QStringLiteral("attributeValue")).toString());
    }
    return QVariant::fromValue(list);
}

QVariant ResourceFileFetcher::getEditionModel(const QString &subject)
{
    QStringList list;
    const auto categoryAttributeSubjectTrees(catalogDetail
            .value(QStringLiteral("data")).toObject()
            .value(QStringLiteral("categoryAttributeTreeList")).toArray()
            .at(0).toObject()
            .value(QStringLiteral("categoryAttributeTrees")).toArray());
    for(const auto &i : categoryAttributeSubjectTrees)
    {
        const auto subjectObject(i.toObject());
        if(subjectObject.value(QStringLiteral("attributeValue")).toString() == subject)
        {
            const auto categoryAttributeEditionTrees(subjectObject.value(QStringLiteral("categoryAttributeTrees")).toArray());
            for(const auto &j : categoryAttributeEditionTrees)
            {
                list.append(j.toObject().value(QStringLiteral("attributeValue")).toString());
            }
        }
    }
    return QVariant::fromValue(list);
}

QVariant ResourceFileFetcher::getModuleModel(const QString &subject, const QString &edition)
{
    QStringList list;
    const auto categoryAttributeSubjectTrees(catalogDetail
            .value(QStringLiteral("data")).toObject()
            .value(QStringLiteral("categoryAttributeTreeList")).toArray()
            .at(0).toObject()
            .value(QStringLiteral("categoryAttributeTrees")).toArray());
    // 什么嵌套天堂
    for(const auto &i : categoryAttributeSubjectTrees)
    {
        const auto subjectObject(i.toObject());
        if(subjectObject.value(QStringLiteral("attributeValue")).toString() == subject)
        {
            const auto categoryAttributeEditionTrees(subjectObject.value(QStringLiteral("categoryAttributeTrees")).toArray());
            for(const auto &j : categoryAttributeEditionTrees)
            {
                const auto editionObject(j.toObject());
                if(editionObject.value(QStringLiteral("attributeValue")).toString() == edition)
                {
                    const auto categoryAttributeModuleTrees(editionObject.value(QStringLiteral("categoryAttributeTrees")).toArray());
                    for(const auto &k : categoryAttributeModuleTrees)
                    {
                        list.append(k.toObject().value(QStringLiteral("attributeValue")).toString());
                    }
                }
            }
        }
    }
    return QVariant::fromValue(list);
}

bool ResourceFileFetcher::haveNextPage() const
{
    return currentPage < maxPage;
}

void ResourceFileFetcher::onCatalogArrayReplyFinished()
{
    auto reply(qobject_cast<QNetworkReply *>(sender()));
    auto rawData(reply->readAll());
    auto jsonObject(QJsonDocument::fromJson(rawData).object());
    if(jsonObject.value(QStringLiteral("code")).toInt() != 200)
    {
        emit error(reply->request().url().toString().append(rawData));
    }
    reply->deleteLater();

    auto catalogArray(jsonObject.value(QStringLiteral("data")).toArray());

    auto postData(QJsonDocument(catalogArray).toJson(QJsonDocument::Compact));

    auto nextReply(Network::getGlobalNetworkManager()->post(setRequest(QStringLiteral("https://www.xinjiaoyu.com/api/v3/server_questions/book/catalog/detail/attribute")), postData));
    connect(nextReply, &QNetworkReply::finished, this, &ResourceFileFetcher::onCatalogDetailReplyFinished);
}

void ResourceFileFetcher::onCatalogDetailReplyFinished()
{
    auto reply(qobject_cast<QNetworkReply *>(sender()));
    auto rawData(reply->readAll());
    catalogDetail = QJsonDocument::fromJson(rawData).object();
    if(catalogDetail.value(QStringLiteral("code")).toInt() != 200)
    {
        emit error(reply->request().url().toString().append(rawData));
    }
    reply->deleteLater();
    emit initFinished();
}

void ResourceFileFetcher::onCatalogReplyFinished()
{
    auto reply(qobject_cast<QNetworkReply *>(sender()));
    auto rawData(reply->readAll());
    reply->deleteLater();

    const auto schoolIdIndex(rawData.indexOf(QByteArrayLiteral("100117")));
    const auto bookCatalogIdIndex(rawData.lastIndexOf(QByteArrayLiteral("bookCatalogId"), schoolIdIndex));
    const auto bookCatalogIdValBeginIndex(bookCatalogIdIndex + 16);
    const auto bookCatalogIdValEndIndex(rawData.indexOf(QByteArrayLiteral("\""), bookCatalogIdValBeginIndex));
    currentBookCatalogId = rawData.mid(bookCatalogIdValBeginIndex, bookCatalogIdValEndIndex - bookCatalogIdValBeginIndex);

    qDebug() << Q_FUNC_INFO << "currentBookCatalogId:" << currentBookCatalogId;

    auto nextReply(Network::getGlobalNetworkManager()->get(setRequest(QStringLiteral("https://www.xinjiaoyu.com/api/v3/server_resource/homework/resource?bookCatalogId=%0&current=1&size=10").arg(currentBookCatalogId))));
    connect(nextReply, &QNetworkReply::finished, this, &ResourceFileFetcher::onResourceReplyFinished);
}

void ResourceFileFetcher::onResourceReplyFinished()
{
    auto reply(qobject_cast<QNetworkReply *>(sender()));
    auto rawData(reply->readAll());
    auto jsonObject(QJsonDocument::fromJson(rawData).object());
    if(jsonObject.value(QStringLiteral("code")).toInt() != 200)
    {
        emit error(reply->request().url().toString().append(rawData));
    }
    reply->deleteLater();
    maxPage = jsonObject.value(QStringLiteral("data")).toObject().value(QStringLiteral("pages")).toString().toInt();
    qDebug() << "maxPage:" << maxPage;

    QJsonArray newRecords(jsonObject.value(QStringLiteral("data")).toObject().value(QStringLiteral("records")).toArray());

    if(!model->records.isEmpty())
    {
        model->beginRemoveRows(QModelIndex(), 0, model->records.size() - 1);
        model->records = QJsonArray();
        model->endRemoveRows();
    }
    if(!newRecords.isEmpty())
    {
        model->beginInsertRows(QModelIndex(), 0, newRecords.size() - 1);
        model->records = std::move(newRecords);
        model->endInsertRows();
    }

    emit resetModelFinished();
}

void ResourceFileFetcher::onContinueLoadResourceReplyFinished()
{
    auto reply(qobject_cast<QNetworkReply *>(sender()));
    auto rawData(reply->readAll());
    auto jsonObject(QJsonDocument::fromJson(rawData).object());
    if(jsonObject.value(QStringLiteral("code")).toInt() != 200)
    {
        emit error(reply->request().url().toString().append(rawData));
    }
    reply->deleteLater();

    QJsonArray newRecords(jsonObject.value(QStringLiteral("data")).toObject().value(QStringLiteral("records")).toArray());

    model->beginInsertRows(QModelIndex(), model->records.size(), model->records.size() + newRecords.size() - 1);
    for(const auto &i : newRecords)
        model->records.append(i.toObject());
    model->endInsertRows();

    emit continueLoadModelFinished();
}

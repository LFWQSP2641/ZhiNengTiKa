#include "AccountManager.h"
#include "../Singleton/Network.h"
#include "../StaticClass/XinjiaoyuEncryptioner.h"
#include "../StaticClass/XinjiaoyuNetwork.h"

AccountManager::AccountManager(QObject *parent)
    : QObject{parent},
      publicUserData(UserData())
{
    publicUserData.publicUserData = true;
    initPublicUserData();
}

void AccountManager::login(const QString &username, const QString &password)
{
    const auto utf8Username(username.toUtf8());
    const auto utf8Password(password.toUtf8());
    loginUtf8(utf8Username, utf8Password);
}

void AccountManager::loginUtf8(const QByteArray &username, const QByteArray &password)
{
    connect(getLoginReplyUtf8(username, password), &QNetworkReply::finished, this, &AccountManager::onLoginReplyFinished);
}

void AccountManager::relogin(qsizetype index)
{
    const auto userData(this->at(index));
    auto reply(getLoginReplyUtf8(userData.getUsername(), userData.getPassword()));
    connect(reply, &QNetworkReply::finished, this, &AccountManager::onReloginReplyFinished);
    reloginHash.insert(reply, userData);
}

void AccountManager::checkCurrentAccountValid()
{
    auto request{XinjiaoyuNetwork::setRequest(QStringLiteral("https://www.xinjiaoyu.com/api/v3/server_system/member/user/vip"), this->getCurrentUserData())};
    const auto reply{Network::getGlobalNetworkManager()->get(request)};
    connect(reply, &QNetworkReply::finished, this, &AccountManager::onCheckCurrentAccountValidReplyFinished);
}

void AccountManager::initPublicUserData()
{
    auto reply { Network::getGlobalNetworkManager()->get(QNetworkRequest(QUrl(QStringLiteral("PublicUserData/").prepend(DATABASE_DOMAIN).append(QStringLiteral("publicUserData"))))) };
    QObject::connect(reply, &QNetworkReply::finished, this, &AccountManager::onPublicUserDataReplyFinished);
}

void AccountManager::toFirst(qsizetype i)
{
    const auto currentUserData{ this->at(i) };
    for (; i > 0; --i)
    {
        this->begin()[i] = this->at(i - 1);
    }
    this->begin()[0] = currentUserData;
}

void AccountManager::removeFirst()
{
    this->QList::removeFirst();
}

void AccountManager::logout()
{
    removeFirst();
}

QNetworkReply *AccountManager::getLoginReply(const QString &username, const QString &password)
{
    const auto utf8Username(username.toUtf8());
    const auto utf8Password(password.toUtf8());
    return getLoginReplyUtf8(utf8Username, utf8Password);
}

QNetworkReply *AccountManager::getLoginReplyUtf8(const QByteArray &username, const QByteArray &password)
{
    const auto encodedUsername{XinjiaoyuEncryptioner::xinjiaoyuEncryption(username)};
    const auto encodedPassword{XinjiaoyuEncryptioner::xinjiaoyuEncryption(password)};
    const auto loginData(QStringLiteral("{\"password\":\"%0\",\"t\":%1,\"username\":\"%2\"}").arg(encodedPassword, QString::number(QDateTime::currentMSecsSinceEpoch()), encodedUsername).toUtf8());
    QNetworkRequest loginRequest(QStringLiteral("https://www.xinjiaoyu.com/api/v3/server_system/auth/login"));
    loginRequest.setHeader(QNetworkRequest::ContentLengthHeader, loginData.size());
    loginRequest.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json; charset=UTF-8"));

    const auto tVal{QString::number(QDateTime::currentMSecsSinceEpoch()).toUtf8()};
    const auto clientSessionVal{QUuid::createUuid().toString(QUuid::WithoutBraces).toUtf8()};
    const auto encryptVal{XinjiaoyuEncryptioner::getXinjiaoyuMD5(tVal, clientSessionVal)};
    loginRequest.setRawHeader(QByteArrayLiteral("Accept-Encoding"), QByteArrayLiteral("gzip"));
    loginRequest.setRawHeader(QByteArrayLiteral("app"), QByteArrayLiteral("student"));
    loginRequest.setRawHeader(QByteArrayLiteral("Authorization"), QByteArrayLiteral("JBY"));
    loginRequest.setRawHeader(QByteArrayLiteral("client"), QByteArrayLiteral("android"));
    loginRequest.setRawHeader(QByteArrayLiteral("clientSession"), clientSessionVal);
    loginRequest.setRawHeader(QByteArrayLiteral("Content-Type"), QByteArrayLiteral("application/json"));
    loginRequest.setRawHeader(QByteArrayLiteral("User-Agent"), QByteArrayLiteral("okhttp/4.9.3"));
    loginRequest.setRawHeader(QByteArrayLiteral("t"), tVal);
    loginRequest.setRawHeader(QByteArrayLiteral("encrypt"), encryptVal);

    auto list(QList<QByteArray>() << username << password << clientSessionVal);
    auto reply(Network::getGlobalNetworkManager()->post(loginRequest, loginData));
    loginHash.insert(reply, list);
    return reply;
}

UserData AccountManager::getPublicUserData() const
{
    return publicUserData;
}

UserData AccountManager::getCurrentUserData() const
{
    if(isLoggedin())
        return this->at(0);
    else
        return getPublicUserData();
}

bool AccountManager::isLoggedin() const
{
    return (!this->isEmpty());
}

QVariant AccountManager::getDescriptionOfUserDatas() const
{
    QStringList descriptionList;
    if(isLoggedin())
    {
        for(const auto &i : *this)
        {
            descriptionList.append(i.getDescription());
        }
    }
    else
    {
        descriptionList.append(publicUserData.getDescription());
    }
    return QVariant::fromValue(descriptionList);
}

void AccountManager::onLoginReplyFinished()
{
    auto reply(qobject_cast<QNetworkReply *>(sender()));
    const auto returnData{reply->readAll()};
    reply->deleteLater();
    auto list(loginHash.take(reply));
    if(returnData.startsWith("{\"code\":200,"))
    {
        const auto rootObject{QJsonDocument::fromJson(returnData).object()};
        auto dataObject{rootObject.value(QStringLiteral("data")).toObject()};
        const auto userDataJsonObject = dataObject.value(QStringLiteral("info")).toObject();
        const auto userSchoolDataJsonObject = userDataJsonObject.value(QStringLiteral("school")).toObject();
        UserData userData(
            dataObject.value(QStringLiteral("accessToken")).toString().toUtf8(),
            QByteArrayLiteral("JBY ").append(dataObject.value(QStringLiteral("token")).toString().toUtf8()),
            list.at(2),
            userSchoolDataJsonObject.value(QStringLiteral("studentId")).toString().toUtf8(),
            userDataJsonObject, list.at(1),
            userSchoolDataJsonObject.value(QStringLiteral("schoolId")).toString().toUtf8(),
            list.at(0));
        this->prepend(userData);
        emit loginFinished(true, userData);
    }
    else
    {
        emit loginFinished(false, UserData());
        emit error(QStringLiteral("登录失败\n"
                                  "服务器返回信息:\n").append(returnData));
    }
}

void AccountManager::onReloginReplyFinished()
{
    auto reply(qobject_cast<QNetworkReply *>(sender()));
    const auto returnData{reply->readAll()};
    reply->deleteLater();
    auto list(loginHash.take(reply));
    auto userData(reloginHash.take(reply));
    if(returnData.startsWith("{\"code\":200,"))
    {
        const auto rootObject{QJsonDocument::fromJson(returnData).object()};
        auto dataObject{rootObject.value(QStringLiteral("data")).toObject()};
        const auto userDataJsonObject = dataObject.value(QStringLiteral("info")).toObject();
        const auto userSchoolDataJsonObject = userDataJsonObject.value(QStringLiteral("school")).toObject();
        userData.setAccessToken(dataObject.value(QStringLiteral("accessToken")).toString().toUtf8());
        userData.setAuthorization(QByteArrayLiteral("JBY ").append(dataObject.value(QStringLiteral("token")).toString().toUtf8()));
        userData.setClientSession(list.at(2));
        userData.setStudentId(userSchoolDataJsonObject.value(QStringLiteral("studentId")).toString().toUtf8());
        userData.setDetailDataJsonObject(userDataJsonObject);
        userData.setPassword(list.at(1));
        userData.setSchoolId(userSchoolDataJsonObject.value(QStringLiteral("schoolId")).toString().toUtf8());
        userData.setUsername(list.at(0));
        emit reloginFinished(true, userData);
    }
    else
    {
        emit reloginFinished(false, UserData());
        emit error(QStringLiteral("重新登录失败\n"
                                  "服务器返回信息:\n").append(returnData));
    }
}

void AccountManager::onCheckCurrentAccountValidReplyFinished()
{
    auto reply(qobject_cast<QNetworkReply *>(sender()));
    const auto returnData(reply->readAll());
    reply->deleteLater();
    qDebug() << QString(returnData);
    emit checkCurrentAccountValidFinished(returnData.startsWith("{\"code\":200,"));
}

void AccountManager::onPublicUserDataReplyFinished()
{
    auto reply(qobject_cast<QNetworkReply *>(sender()));
    if(reply->error() != QNetworkReply::NoError)
    {
        reply->deleteLater();
        return;
    }
    const auto publicUserDataJson(QJsonDocument::fromJson(Network::getGlobalNetworkManager()->replyReadAll(reply)).object());
    publicUserData.setAccessToken(publicUserDataJson.value(QStringLiteral("accessToken")).toString().toUtf8());
    publicUserData.setAuthorization(publicUserDataJson.value(QStringLiteral("authorization")).toString().toUtf8());
    publicUserData.setClientSession(publicUserDataJson.value(QStringLiteral("clientSession")).toString().toUtf8());
    publicUserData.setStudentId(publicUserDataJson.value(QStringLiteral("studentId")).toString().toUtf8());

    emit initPublicUserDataFinished(publicUserData);
}

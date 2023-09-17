#include "UserData.h"
#include "../StaticClass/XinjiaoyuEncryptioner.h"
#include "../StaticClass/XinjiaoyuNetwork.h"
#include "../Singleton/Network.h"

QByteArray UserData::publicAccessToken;
QByteArray UserData::publicAuthorization;
QByteArray UserData::publicClientSession;
QByteArray UserData::publicStudentId;

UserData UserData::login(const QByteArray &username, const QByteArray &password)
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

    const auto returnData{Network::getGlobalNetworkManager()->postData(loginRequest, loginData)};
    if(returnData.startsWith("{\"code\":200,"))
    {
        const auto rootObject{QJsonDocument::fromJson(returnData).object()};
        auto dataObject{rootObject.value(QStringLiteral("data")).toObject()};
        const auto userDataJsonObject = dataObject.value(QStringLiteral("info")).toObject();
        const auto userSchoolDataJsonObject = userDataJsonObject.value(QStringLiteral("school")).toObject();
        return UserData(
                   dataObject.value(QStringLiteral("accessToken")).toString().toUtf8(),
                   QByteArrayLiteral("JBY ") + dataObject.value(QStringLiteral("token")).toString().toUtf8(),
                   clientSessionVal,
                   userSchoolDataJsonObject.value(QStringLiteral("studentId")).toString().toUtf8(),
                   userDataJsonObject, password,
                   userSchoolDataJsonObject.value(QStringLiteral("schoolId")).toString().toUtf8(),
                   username);
    }
    else
    {
        UserData userData;
        userData.errorStr = QStringLiteral("登录失败\n"
                                           "服务器返回信息:").append(returnData);
        return userData;
    }
}

void UserData::initPublicUserData()
{
    auto reply { Network::getGlobalNetworkManager()->get(QNetworkRequest(QUrl(QStringLiteral("PublicUserData/").prepend(DATABASE_DOMAIN).append(QStringLiteral("publicUserData"))))) };
    QObject::connect(reply, &QNetworkReply::finished, [reply]
    {
        if(reply->error() != QNetworkReply::NoError)
        {
            reply->deleteLater();
            return;
        }
        const auto publicUserDataJson(QJsonDocument::fromJson(Network::getGlobalNetworkManager()->replyReadAll(reply)).object());
        publicAccessToken = publicUserDataJson.value(QStringLiteral("accessToken")).toString().toUtf8();
        publicAuthorization = publicUserDataJson.value(QStringLiteral("authorization")).toString().toUtf8();
        publicClientSession = publicUserDataJson.value(QStringLiteral("clientSession")).toString().toUtf8();
        publicStudentId = publicUserDataJson.value(QStringLiteral("studentId")).toString().toUtf8();
    });
}

UserData UserData::getPublicUserData()
{
    return UserData(UserData::publicAccessToken,
                    UserData::publicAuthorization,
                    UserData::publicClientSession,
                    UserData::publicStudentId);
}

bool UserData::relogin()
{
    const auto newUserData(UserData::login(this->getUsername(), this->getPassword()));
    if(newUserData.isEmpty())
    {
        this->errorStr = newUserData.getErrorStr();
        return false;
    }
    this->accessToken = newUserData.getAccessToken();
    this->authorization = newUserData.getAuthorization();
    this->clientSession = newUserData.getClientSession();
    this->detailDataJsonObject = newUserData.getDetailDataJsonObject();
    this->schoolId = newUserData.getSchoolId();
    this->studentId = newUserData.getStudentId();
    return true;
}

QString UserData::getErrorStr() const
{
    return errorStr;
}

bool UserData::isValid() const
{
    auto request{XinjiaoyuNetwork::setRequest(QStringLiteral("https://www.xinjiaoyu.com/api/v3/server_system/member/user/vip"), *this)};
    const auto returnData{Network::getGlobalNetworkManager()->getData(request)};
    return returnData.startsWith("{\"code\":200,");
}

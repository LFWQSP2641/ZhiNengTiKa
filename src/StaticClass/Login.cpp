#include "Login.h"
#include "XinjiaoyuEncryptioner.h"
#include "Network.h"
#include "Setting.h"

bool Login::userLogined;


QByteArray Login::login(const QByteArray &username, const QByteArray &password)
{
    const auto encodedUsername{XinjiaoyuEncryptioner::xinjiaoyuEncryption(username)};
    const auto encodedPassword{XinjiaoyuEncryptioner::xinjiaoyuEncryption(password)};
    const auto loginData(QStringLiteral("{\"password\":\"%0\",\"t\":%1,\"username\":\"%2\"}").arg(encodedPassword, QString::number(QDateTime::currentMSecsSinceEpoch()), encodedUsername).toUtf8());
    QNetworkRequest loginRequest(QStringLiteral("https://www.xinjiaoyu.com/api/v3/server_system/auth/login"));
    loginRequest.setHeader(QNetworkRequest::ContentLengthHeader, loginData.size());
    loginRequest.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json; charset=UTF-8"));
    return Network::postData(loginRequest, loginData);
}

Login::loginState Login::refreshUserData()
{
    Login::refreshClientSession();
    if(Setting::userPassword.isEmpty() || Setting::userUsername.isEmpty())
    {
        return Login::loginState::SomethingIsEmpty;
    }
    const auto returnData{Login::login(Setting::userUsername, Setting::userPassword)};
    const auto rootObject{QJsonDocument::fromJson(returnData).object()};
    auto dataObject{rootObject.value(QStringLiteral("data")).toObject()};
    switch (rootObject.value(QStringLiteral("code")).toInt())
    {
    case 200:
        Setting::userAuthorization = QString(QStringLiteral("JBY ") + dataObject.value(QStringLiteral("token")).toString()).toUtf8();
        Setting::userAccessToken = QString(dataObject.value(QStringLiteral("accessToken")).toString()).toUtf8();
        Setting::userStudentId = dataObject.value(QStringLiteral("info")).toObject().value(QStringLiteral("school")).toObject().value(QStringLiteral("studentId")).toString().toUtf8();
        Setting::userSchoolId = dataObject.value(QStringLiteral("info")).toObject().value(QStringLiteral("school")).toObject().value(QStringLiteral("schoolId")).toString().toUtf8();
        Setting::userData = dataObject.value(QStringLiteral("info")).toObject();
#ifdef Q_OS_ANDROID
        Setting::saveToFile();
#endif // Q_OS_ANDROID
        Login::userLogined = true;
        return Login::loginState::Success;
        break;
    case 400:
        Login::userLogined = false;
        return Login::loginState::SomethingIsEmpty;
        break;
    case 414:
        Login::userLogined = false;
        return Login::loginState::SomethingIsWrong;
        break;
    case 415:
        Login::userLogined = false;
        return Login::loginState::SomethingIsWrong;
        break;
    default:
        Login::userLogined = false;
        return Login::loginState::error;
        break;
    }
}

Login::loginState Login::refreshUserData(const QByteArray &username, const QByteArray &password)
{
    const auto oldUsername{Setting::userUsername};
    const auto oldPassword{Setting::userPassword};
    Setting::userUsername = username;
    Setting::userPassword = password;
    const auto ret{Login::refreshUserData()};
    if(ret != Login::loginState::Success)
    {
        Setting::userUsername = oldUsername;
        Setting::userPassword = oldPassword;
    }
    return ret;
}

void Login::refreshClientSession()
{
    Setting::userClientSession = QUuid::createUuid().toString(QUuid::WithoutBraces).toUtf8();
}

void Login::userLogout()
{
    Login::userLogined = false;
    Setting::userUsername.clear();
    Setting::userPassword.clear();
    Setting::userSchoolId.clear();
    Setting::userStudentId.clear();
    Setting::userAuthorization.clear();
    Setting::userAccessToken.clear();
    Setting::userData = QJsonObject();
#ifdef Q_OS_ANDROID
    Setting::saveToFile();
#endif
}

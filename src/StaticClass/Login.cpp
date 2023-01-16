#include "Login.h"
#include "XinjiaoyuEncryptioner.h"
#include "Network.h"
#include "Setting.h"

QByteArray Login::login(const QByteArray &username, const QByteArray &password)
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

    const auto returnData{Network::postData(loginRequest, loginData)};
    if(returnData.startsWith("{\"code\":200,"))
    {
        const auto rootObject{QJsonDocument::fromJson(returnData).object()};
        auto dataObject{rootObject.value(QStringLiteral("data")).toObject()};
        const auto userDataJsonObject = dataObject.value(QStringLiteral("info")).toObject();
        const auto userSchoolDataJsonObject = userDataJsonObject.value(QStringLiteral("school")).toObject();
        Setting::userDataList.prepend(UserData(
                                          dataObject.value(QStringLiteral("accessToken")).toString().toUtf8(),
                                          QByteArrayLiteral("JBY ") + dataObject.value(QStringLiteral("token")).toString().toUtf8(),
                                          clientSessionVal,
                                          password,
                                          userSchoolDataJsonObject.value(QStringLiteral("schoolId")).toString().toUtf8(),
                                          userDataJsonObject,
                                          userSchoolDataJsonObject.value(QStringLiteral("studentId")).toString().toUtf8(),
                                          username
                                      ));
#ifdef Q_OS_ANDROID
        Setting::saveToFile();
#endif // Q_OS_ANDROID
    }
    return returnData;
}

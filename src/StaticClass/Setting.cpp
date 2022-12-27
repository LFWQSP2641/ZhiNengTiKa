#include "Setting.h"
#include "Global.h"
#include "Login.h"

QJsonObject Setting::settingJsonObject;

//负责题卡的获取
const QByteArray Setting::studentId(QByteArrayLiteral("196209"));
const QByteArray Setting::clientSession(QUuid::createUuid().toString(QUuid::WithoutBraces).toUtf8());
QByteArray Setting::Authorization;
QByteArray Setting::accessToken;
//负责题卡的上传
QByteArray Setting::userStudentId;
QByteArray Setting::userClientSession;
QByteArray Setting::userSchoolId;
QByteArray Setting::userAuthorization;
QByteArray Setting::userAccessToken;
QByteArray Setting::userUsername;
QByteArray Setting::userPassword;
QJsonObject Setting::userData;
bool Setting::listAllTemplate;
//若这三为空,就在main那赋个默认值
int Setting::fontPointSize;
int Setting::smallFontPointSize;
QString Setting::font;

void Setting::loadFromFile()
{
    QByteArray fileData;
    QFile file{ Global::configPath() + QDir::separator() + QStringLiteral("setting.json") };
    if (file.exists())
    {
        file.open(QFile::ReadOnly);
        fileData = file.readAll();
    }
    else
    {
        file.open(QFile::NewOnly);
    }
    file.close();

    auto object{QJsonDocument::fromJson(fileData).object()};

    Setting::settingJsonObject = object;
    Setting::Authorization = object.value(QStringLiteral("Authorization")).toString().toUtf8();
    Setting::accessToken = object.value(QStringLiteral("accessToken")).toString().toUtf8();
    Setting::userSchoolId = object.value(QStringLiteral("userSchoolId")).toString().toUtf8();
    Setting::userStudentId = object.value(QStringLiteral("userStudentId")).toString().toUtf8();
    Setting::userAuthorization = object.value(QStringLiteral("userAuthorization")).toString().toUtf8();
    Setting::userAccessToken = object.value(QStringLiteral("userAccessToken")).toString().toUtf8();
    Setting::userUsername = object.value(QStringLiteral("userUsername")).toString().toUtf8();
    Setting::userPassword = object.value(QStringLiteral("userPassword")).toString().toUtf8();
    Setting::userData = QJsonDocument::fromJson(QByteArray::fromBase64(object.value(QStringLiteral("userData")).toString().toUtf8())).object();
    Setting::listAllTemplate = object.value(QStringLiteral("listAllTemplate")).toBool(false);
    Setting::fontPointSize = object.value(QStringLiteral("fontPointSize")).toInt();
    Setting::smallFontPointSize = object.value(QStringLiteral("smallFontPointSize")).toInt();
    Setting::font = object.value(QStringLiteral("font")).toString();

    if(!Setting::userAuthorization.isEmpty())
    {
        Login::userLogined = true;
    }
    else
    {
        Login::userLogined = false;
    }
    Login::refreshUserClientSession();
}

void Setting::saveToFile()
{
    Setting::settingJsonObject.insert(QStringLiteral("Authorization"), QString(Setting::Authorization));
    Setting::settingJsonObject.insert(QStringLiteral("accessToken"), QString(Setting::accessToken));
    Setting::settingJsonObject.insert(QStringLiteral("userSchoolId"), QString(Setting::userSchoolId));
    Setting::settingJsonObject.insert(QStringLiteral("userStudentId"), QString(Setting::userStudentId));
    Setting::settingJsonObject.insert(QStringLiteral("userAuthorization"), QString(Setting::userAuthorization));
    Setting::settingJsonObject.insert(QStringLiteral("userAccessToken"), QString(Setting::userAccessToken));
    Setting::settingJsonObject.insert(QStringLiteral("userUsername"), QString(Setting::userUsername));
    Setting::settingJsonObject.insert(QStringLiteral("userPassword"), QString(Setting::userPassword));
    Setting::settingJsonObject.insert(QStringLiteral("userData"), QString(QJsonDocument(Setting::userData).toJson(QJsonDocument::Compact).toBase64()));
    Setting::settingJsonObject.insert(QStringLiteral("listAllTemplate"), listAllTemplate);
    Setting::settingJsonObject.insert(QStringLiteral("fontPointSize"), fontPointSize);
    Setting::settingJsonObject.insert(QStringLiteral("smallFontPointSize"), smallFontPointSize);
    Setting::settingJsonObject.insert(QStringLiteral("font"), font);

    QFile file{ Global::configPath() + QDir::separator() + QStringLiteral("setting.json") };
    file.open(QFile::WriteOnly);
    file.write(QJsonDocument(Setting::settingJsonObject).toJson(QJsonDocument::Compact));
    file.close();
}

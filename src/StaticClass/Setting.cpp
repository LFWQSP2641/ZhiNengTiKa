#include "Setting.h"
#include "Global.h"
#include "Login.h"

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

    const auto settingJsonObject{QJsonDocument::fromJson(fileData).object()};

    Setting::userSchoolId = settingJsonObject.value(QStringLiteral("userSchoolId")).toString().toUtf8();
    Setting::userStudentId = settingJsonObject.value(QStringLiteral("userStudentId")).toString().toUtf8();
    Setting::userAuthorization = settingJsonObject.value(QStringLiteral("userAuthorization")).toString().toUtf8();
    Setting::userAccessToken = settingJsonObject.value(QStringLiteral("userAccessToken")).toString().toUtf8();
    Setting::userUsername = settingJsonObject.value(QStringLiteral("userUsername")).toString().toUtf8();
    Setting::userPassword = settingJsonObject.value(QStringLiteral("userPassword")).toString().toUtf8();
    Setting::userData = QJsonDocument::fromJson(QByteArray::fromBase64(settingJsonObject.value(QStringLiteral("userData")).toString().toUtf8())).object();

    Setting::listAllTemplate = settingJsonObject.value(QStringLiteral("listAllTemplate")).toBool(false);
    Setting::fontPointSize = settingJsonObject.value(QStringLiteral("fontPointSize")).toInt();
    Setting::smallFontPointSize = settingJsonObject.value(QStringLiteral("smallFontPointSize")).toInt();
    Setting::font = settingJsonObject.value(QStringLiteral("font")).toString();

    if(!Setting::userAuthorization.isEmpty())
    {
        Login::userLogined = true;
    }
    else
    {
        Login::userLogined = false;
    }
    Login::refreshClientSession();
}

void Setting::saveToFile()
{
    QJsonObject settingJsonObject;
    settingJsonObject.insert(QStringLiteral("userSchoolId"), QString(Setting::userSchoolId));
    settingJsonObject.insert(QStringLiteral("userStudentId"), QString(Setting::userStudentId));
    settingJsonObject.insert(QStringLiteral("userAuthorization"), QString(Setting::userAuthorization));
    settingJsonObject.insert(QStringLiteral("userAccessToken"), QString(Setting::userAccessToken));
    settingJsonObject.insert(QStringLiteral("userUsername"), QString(Setting::userUsername));
    settingJsonObject.insert(QStringLiteral("userPassword"), QString(Setting::userPassword));
    settingJsonObject.insert(QStringLiteral("userData"), QString(QJsonDocument(Setting::userData).toJson(QJsonDocument::Compact).toBase64()));

    settingJsonObject.insert(QStringLiteral("listAllTemplate"), listAllTemplate);
    settingJsonObject.insert(QStringLiteral("fontPointSize"), fontPointSize);
    settingJsonObject.insert(QStringLiteral("smallFontPointSize"), smallFontPointSize);
    settingJsonObject.insert(QStringLiteral("font"), font);

    QFile file{ Global::configPath() + QDir::separator() + QStringLiteral("setting.json") };
    file.open(QFile::WriteOnly);
    file.write(QJsonDocument(settingJsonObject).toJson(QJsonDocument::Compact));
    file.close();
}

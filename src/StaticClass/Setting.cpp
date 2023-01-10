#include "Setting.h"
#include "Global.h"
#include "Login.h"

UserDataList Setting::userDataList;

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
    const auto accountsJsonArray{settingJsonObject.value("accounts").toArray()};

    for(const auto &i : accountsJsonArray)
    {
        const auto jsonObject{i.toObject()};
        userDataList.append(UserData(
                                jsonObject.value(QStringLiteral("AccessToken")).toString().toUtf8(),
                                jsonObject.value(QStringLiteral("Authorization")).toString().toUtf8(),
                                jsonObject.value(QStringLiteral("ClientSession")).toString().toUtf8(),
                                jsonObject.value(QStringLiteral("Password")).toString().toUtf8(),
                                jsonObject.value(QStringLiteral("SchoolId")).toString().toUtf8(),
                                QJsonDocument::fromJson(QByteArray::fromBase64(jsonObject.value(QStringLiteral("SheetData")).toString().toUtf8())).object(),
                                jsonObject.value(QStringLiteral("StudentId")).toString().toUtf8(),
                                jsonObject.value(QStringLiteral("Username")).toString().toUtf8()));
    }

    Setting::listAllTemplate = settingJsonObject.value(QStringLiteral("listAllTemplate")).toBool(false);
    Setting::fontPointSize = settingJsonObject.value(QStringLiteral("fontPointSize")).toInt();
    Setting::smallFontPointSize = settingJsonObject.value(QStringLiteral("smallFontPointSize")).toInt();
    Setting::font = settingJsonObject.value(QStringLiteral("font")).toString();
}

void Setting::saveToFile()
{
    QJsonObject settingJsonObject;
    QJsonArray accountsJsonArray;
    for(const auto &i : userDataList)
    {
        QJsonObject jsonObject;
        jsonObject.insert(QStringLiteral("AccessToken"), QString(i.accessToken()));
        jsonObject.insert(QStringLiteral("Authorization"), QString(i.authorization()));
        jsonObject.insert(QStringLiteral("ClientSession"), QString(i.clientSession()));
        jsonObject.insert(QStringLiteral("Password"), QString(i.password()));
        jsonObject.insert(QStringLiteral("SchoolId"), QString(i.schoolId()));
        jsonObject.insert(QStringLiteral("SheetData"), QString(QJsonDocument(i.sheetData()).toJson(QJsonDocument::Compact).toBase64()));
        jsonObject.insert(QStringLiteral("StudentId"), QString(i.studentId()));
        jsonObject.insert(QStringLiteral("Username"), QString(i.username()));

        accountsJsonArray.append(jsonObject);
    }
    settingJsonObject.insert(QStringLiteral("accounts"), accountsJsonArray);

    settingJsonObject.insert(QStringLiteral("listAllTemplate"), listAllTemplate);
    settingJsonObject.insert(QStringLiteral("fontPointSize"), fontPointSize);
    settingJsonObject.insert(QStringLiteral("smallFontPointSize"), smallFontPointSize);
    settingJsonObject.insert(QStringLiteral("font"), font);

    QFile file{ Global::configPath() + QDir::separator() + QStringLiteral("setting.json") };
    file.open(QFile::WriteOnly);
    file.write(QJsonDocument(settingJsonObject).toJson(QJsonDocument::Compact));
    file.close();
}

QByteArray Setting::accessToken()
{
    return Setting::userDataList.at(0).accessToken();
}

QByteArray Setting::authorization()
{
    return Setting::userDataList.at(0).authorization();
}

QByteArray Setting::clientSession()
{
    return Setting::userDataList.at(0).clientSession();
}

QByteArray Setting::password()
{
    return Setting::userDataList.at(0).password();
}

QByteArray Setting::schoolId()
{
    return Setting::userDataList.at(0).schoolId();
}

QJsonObject Setting::sheetData()
{
    return Setting::userDataList.at(0).sheetData();
}

QByteArray Setting::studentId()
{
    return Setting::userDataList.at(0).studentId();
}

QByteArray Setting::username()
{
    return Setting::userDataList.at(0).username();
}

bool Setting::logined()
{
    return !Setting::userDataList.isEmpty();
}

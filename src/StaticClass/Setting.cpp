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
        //只有authorization不能空(虽然别的空了也不行...)
        if(!jsonObject.contains(QStringLiteral("authorization")))
        {
            continue;
        }
        userDataList.append(UserData(
                                jsonObject.value(QStringLiteral("accessToken")).toString().toUtf8(),
                                jsonObject.value(QStringLiteral("authorization")).toString().toUtf8(),
                                jsonObject.value(QStringLiteral("clientSession")).toString(QUuid::createUuid().toString(QUuid::WithoutBraces)).toUtf8(),
                                jsonObject.value(QStringLiteral("password")).toString().toUtf8(),
                                jsonObject.value(QStringLiteral("schoolId")).toString().toUtf8(),
                                QJsonDocument::fromJson(QByteArray::fromBase64(jsonObject.value(QStringLiteral("sheetData")).toString().toUtf8())).object(),
                                jsonObject.value(QStringLiteral("studentId")).toString().toUtf8(),
                                jsonObject.value(QStringLiteral("username")).toString().toUtf8()));
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
        jsonObject.insert(QStringLiteral("accessToken"), QString(i.accessToken()));
        jsonObject.insert(QStringLiteral("authorization"), QString(i.authorization()));
        jsonObject.insert(QStringLiteral("clientSession"), QString(i.clientSession()));
        jsonObject.insert(QStringLiteral("password"), QString(i.password()));
        jsonObject.insert(QStringLiteral("schoolId"), QString(i.schoolId()));
        jsonObject.insert(QStringLiteral("sheetData"), QString(QJsonDocument(i.sheetData()).toJson(QJsonDocument::Compact).toBase64()));
        jsonObject.insert(QStringLiteral("studentId"), QString(i.studentId()));
        jsonObject.insert(QStringLiteral("username"), QString(i.username()));

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

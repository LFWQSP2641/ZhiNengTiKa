#include "Setting.h"
#include "Global.h"

UserDataList Setting::userDataList;

bool Setting::listLatestTemplatePreferentially;

bool Setting::compressQRCodeImage;

//若这三为空,就在main那赋个默认值
int Setting::fontPointSize;
int Setting::smallFontPointSize;
QString Setting::font;
QString Setting::qmlStyle;

QJsonObject Setting::jsonObjectApiQRCodeScanner;

QString Setting::uuid;

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
                                jsonObject.value(QStringLiteral("studentId")).toString().toUtf8(),
                                QJsonDocument::fromJson(QByteArray::fromBase64(jsonObject.value(QStringLiteral("detailData")).toString().toUtf8())).object(),
                                jsonObject.value(QStringLiteral("password")).toString().toUtf8(),
                                jsonObject.value(QStringLiteral("schoolId")).toString().toUtf8(),
                                jsonObject.value(QStringLiteral("username")).toString().toUtf8()));
    }

    Setting::compressQRCodeImage = settingJsonObject.value(QStringLiteral("compressQRCodeImage")).toBool(true);

    Setting::listLatestTemplatePreferentially = settingJsonObject.value(QStringLiteral("listLatestTemplatePreferentially")).toBool(true);

    Setting::fontPointSize = settingJsonObject.value(QStringLiteral("fontPointSize")).toInt();
    Setting::smallFontPointSize = settingJsonObject.value(QStringLiteral("smallFontPointSize")).toInt();
    Setting::font = settingJsonObject.value(QStringLiteral("font")).toString();
    Setting::qmlStyle = settingJsonObject.value(QStringLiteral("qmlStyle")).toString();

    Setting::jsonObjectApiQRCodeScanner = settingJsonObject.value(QStringLiteral("apiQRCodeScanner")).toObject();
    Setting::uuid = settingJsonObject.value(QStringLiteral("uuid")).toString(QUuid::createUuid().toString(QUuid::WithoutBraces));
}

void Setting::saveToFile()
{
    QJsonObject settingJsonObject;
    QJsonArray accountsJsonArray;
    for(const auto &i : userDataList)
    {
        QJsonObject jsonObject;
        jsonObject.insert(QStringLiteral("accessToken"), QString(i.getAccessToken()));
        jsonObject.insert(QStringLiteral("authorization"), QString(i.getAuthorization()));
        jsonObject.insert(QStringLiteral("clientSession"), QString(i.getClientSession()));
        jsonObject.insert(QStringLiteral("password"), QString(i.getPassword()));
        jsonObject.insert(QStringLiteral("schoolId"), QString(i.getSchoolId()));
        jsonObject.insert(QStringLiteral("detailData"), QString(QJsonDocument(i.getDetailDataJsonObject()).toJson(QJsonDocument::Compact).toBase64()));
        jsonObject.insert(QStringLiteral("studentId"), QString(i.getStudentId()));
        jsonObject.insert(QStringLiteral("username"), QString(i.getUsername()));

        accountsJsonArray.append(jsonObject);
    }
    settingJsonObject.insert(QStringLiteral("accounts"), accountsJsonArray);

    settingJsonObject.insert(QStringLiteral("compressQRCodeImage"), compressQRCodeImage);

    settingJsonObject.insert(QStringLiteral("listLatestTemplatePreferentially"), listLatestTemplatePreferentially);

    settingJsonObject.insert(QStringLiteral("fontPointSize"), fontPointSize);
    settingJsonObject.insert(QStringLiteral("smallFontPointSize"), smallFontPointSize);
    settingJsonObject.insert(QStringLiteral("font"), font);

    settingJsonObject.insert(QStringLiteral("qmlStyle"), qmlStyle);
    settingJsonObject.insert(QStringLiteral("apiQRCodeScanner"), jsonObjectApiQRCodeScanner);
    settingJsonObject.insert(QStringLiteral("uuid"), uuid);

    QFile file{ Global::configPath() + QDir::separator() + QStringLiteral("setting.json") };
    file.open(QFile::WriteOnly);
    file.write(QJsonDocument(settingJsonObject).toJson(QJsonDocument::Compact));
    file.close();
}

bool Setting::logined()
{
    return !Setting::userDataList.isEmpty();
}

UserData Setting::currentUserData()
{
    if(Setting::logined())
    {
        return Setting::userDataList.at(0);
    }
    else
    {
        return UserData::getPublicUserData();
    }
}

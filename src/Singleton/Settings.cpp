#include "Settings.h"
#include "../StaticClass/CallAndroidNativeComponent.h"
#include "../StaticClass/Global.h"
#include "src/Logic/UserData.h"

Settings *Settings::singletonSettings = nullptr;

Settings::Settings(QObject *parent)
    : QObject{parent},
      accountManager(new AccountManager(this))
{
    QByteArray fileData;
    QFile file{ Global::configPath().append(QStringLiteral("/setting.json")) };
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
        accountManager->append(new UserData(
                                   jsonObject.value(QStringLiteral("accessToken")).toString().toUtf8(),
                                   jsonObject.value(QStringLiteral("authorization")).toString().toUtf8(),
                                   jsonObject.value(QStringLiteral("clientSession")).toString(QUuid::createUuid().toString(QUuid::WithoutBraces)).toUtf8(),
                                   jsonObject.value(QStringLiteral("studentId")).toString().toUtf8(),
                                   QJsonDocument::fromJson(QByteArray::fromBase64(jsonObject.value(QStringLiteral("detailData")).toString().toUtf8())).object(),
                                   jsonObject.value(QStringLiteral("password")).toString().toUtf8(),
                                   jsonObject.value(QStringLiteral("schoolId")).toString().toUtf8(),
                                   jsonObject.value(QStringLiteral("username")).toString().toUtf8(),
                                   accountManager));
    }

    compressQRCodeImage = settingJsonObject.value(QStringLiteral("compressQRCodeImage")).toBool(true);

    listLatestTemplatePreferentially = settingJsonObject.value(QStringLiteral("listLatestTemplatePreferentially")).toBool(true);

    fontPointSize = settingJsonObject.value(QStringLiteral("fontPointSize")).toInt();
    font = settingJsonObject.value(QStringLiteral("font")).toString();
    qmlStyle = settingJsonObject.value(QStringLiteral("qmlStyle")).toString();

    jsonObjectApiQRCodeScanner = settingJsonObject.value(QStringLiteral("apiQRCodeScanner")).toObject();

    uuid = settingJsonObject.value(QStringLiteral("uuid")).toString();
    if(uuid.isEmpty())
    {
        resetUuid();
    }

    animeImageUrl =  settingJsonObject.value(QStringLiteral("animeImageUrl")).toString(animeImageUrlList.at(0).second);
}

AccountManager *Settings::getAccountManager() const
{
    return accountManager;
}

QString Settings::getAnimeImageUrl() const
{
    return animeImageUrl;
}

void Settings::setAnimeImageUrl(const QString &newAnimeImageUrl)
{
    if (animeImageUrl == newAnimeImageUrl)
        return;
    animeImageUrl = newAnimeImageUrl;
    emit animeImageUrlChanged();
}

void Settings::resetAnimeImageUrl()
{
    setAnimeImageUrl(animeImageUrlList.at(0).second);
}

QList<QPair<QString, QString> > Settings::getAnimeImageUrlList() const
{
    return animeImageUrlList;
}

void Settings::initOnce()
{
    Settings::singletonSettings = new Settings();
}

void Settings::resetSingletonSettings()
{
    singletonSettings->deleteLater();
    initOnce();
}

Settings *Settings::getSingletonSettings()
{
    return singletonSettings;
}

bool Settings::getListLatestTemplatePreferentially() const
{
    return listLatestTemplatePreferentially;
}

void Settings::setListLatestTemplatePreferentially(bool newListLatestTemplatePreferentially)
{
    if (listLatestTemplatePreferentially == newListLatestTemplatePreferentially)
        return;
    listLatestTemplatePreferentially = newListLatestTemplatePreferentially;
    emit listLatestTemplatePreferentiallyChanged();
}

void Settings::resetListLatestTemplatePreferentially()
{
    setListLatestTemplatePreferentially(true);
}

bool Settings::getCompressQRCodeImage() const
{
    return compressQRCodeImage;
}

void Settings::setCompressQRCodeImage(bool newCompressQRCodeImage)
{
    if (compressQRCodeImage == newCompressQRCodeImage)
        return;
    compressQRCodeImage = newCompressQRCodeImage;
    emit compressQRCodeImageChanged();
}

void Settings::resetCompressQRCodeImage()
{
    setCompressQRCodeImage(true);
}

int Settings::getFontPointSize() const
{
    return fontPointSize;
}

void Settings::setFontPointSize(int newFontPointSize)
{
    if (fontPointSize == newFontPointSize)
        return;
    fontPointSize = newFontPointSize;
    emit fontPointSizeChanged();
}

void Settings::resetFontPointSize()
{
    setFontPointSize(0);
}

QString Settings::getFont() const
{
    return font;
}

void Settings::setFont(const QString &newFont)
{
    if (font == newFont)
        return;
    font = newFont;
    emit fontChanged();
}

void Settings::resetFont()
{
    setFont(QString{});
}

QString Settings::getQmlStyle() const
{
    return qmlStyle;
}

void Settings::setQmlStyle(const QString &newQmlStyle)
{
    if (qmlStyle == newQmlStyle)
        return;
    qmlStyle = newQmlStyle;
    emit qmlStyleChanged();
}

void Settings::resetQmlStyle()
{
    setQmlStyle(QString{});
}

QJsonObject Settings::getJsonObjectApiQRCodeScanner() const
{
    return jsonObjectApiQRCodeScanner;
}

void Settings::setJsonObjectApiQRCodeScanner(const QJsonObject &newJsonObjectApiQRCodeScanner)
{
    if (jsonObjectApiQRCodeScanner == newJsonObjectApiQRCodeScanner)
        return;
    jsonObjectApiQRCodeScanner = newJsonObjectApiQRCodeScanner;
    emit jsonObjectApiQRCodeScannerChanged();
}

void Settings::resetJsonObjectApiQRCodeScanner()
{
    setJsonObjectApiQRCodeScanner(QJsonObject{});
}

QString Settings::getUuid() const
{
    return uuid;
}

void Settings::setUuid(const QString &newUuid)
{
    if (uuid == newUuid)
        return;
    uuid = newUuid;
    emit uuidChanged();
}

void Settings::resetUuid()
{
#ifdef Q_OS_ANDROID
    const auto androidId(CallAndroidNativeComponent::getAndroidId());
    const auto bootUniqueId(QSysInfo::bootUniqueId());
    QString uniqueId;
    if(!androidId.isEmpty() && androidId != QStringLiteral("9774d56d682e549c"))
    {
        uniqueId = androidId;
    }
    else if(!bootUniqueId.isEmpty())
    {
        uniqueId = bootUniqueId;
    }
    if(!uniqueId.isEmpty())
    {
        setUuid(QUuid::createUuidV5(QUuid(), uniqueId).toString(QUuid::WithoutBraces));
    }
    else
    {
        setUuid(QUuid::createUuid().toString(QUuid::WithoutBraces));
    }
#else // Q_OS_ANDROID
    setUuid(QUuid::createUuid().toString(QUuid::WithoutBraces));
#endif // Q_OS_ANDROID
}

void Settings::saveToFile() const
{
    QJsonObject settingJsonObject;
    QJsonArray accountsJsonArray;
    for(const auto &i : *accountManager)
    {
        QJsonObject jsonObject;
        jsonObject.insert(QStringLiteral("accessToken"), QString(i->getAccessToken()));
        jsonObject.insert(QStringLiteral("authorization"), QString(i->getAuthorization()));
        jsonObject.insert(QStringLiteral("clientSession"), QString(i->getClientSession()));
        jsonObject.insert(QStringLiteral("password"), QString(i->getPassword()));
        jsonObject.insert(QStringLiteral("schoolId"), QString(i->getSchoolId()));
        jsonObject.insert(QStringLiteral("detailData"), QString(QJsonDocument(i->getDetailDataJsonObject()).toJson(QJsonDocument::Compact).toBase64()));
        jsonObject.insert(QStringLiteral("studentId"), QString(i->getStudentId()));
        jsonObject.insert(QStringLiteral("username"), QString(i->getUsername()));

        accountsJsonArray.append(jsonObject);
    }
    settingJsonObject.insert(QStringLiteral("accounts"), accountsJsonArray);

    settingJsonObject.insert(QStringLiteral("compressQRCodeImage"), compressQRCodeImage);

    settingJsonObject.insert(QStringLiteral("listLatestTemplatePreferentially"), listLatestTemplatePreferentially);

    settingJsonObject.insert(QStringLiteral("fontPointSize"), fontPointSize);
    settingJsonObject.insert(QStringLiteral("font"), font);

    settingJsonObject.insert(QStringLiteral("qmlStyle"), qmlStyle);
    settingJsonObject.insert(QStringLiteral("apiQRCodeScanner"), jsonObjectApiQRCodeScanner);
    settingJsonObject.insert(QStringLiteral("uuid"), uuid);

    settingJsonObject.insert(QStringLiteral("animeImageUrl"), animeImageUrl);

    QFile file{ Global::configPath().append(QStringLiteral("/setting.json")) };
    file.open(QFile::WriteOnly);
    file.write(QJsonDocument(settingJsonObject).toJson(QJsonDocument::Compact));
    file.close();
}

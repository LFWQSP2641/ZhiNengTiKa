#include "SettingOperator.h"
#include "MultipleSubjectsTemplateListModelListSingleton.h"
#include "../StaticClass/Global.h"
#include "../Singleton/Settings.h"

SettingOperator::SettingOperator(QObject *parent)
    : QObject{parent}
{

}

QByteArray SettingOperator::getCurrentUserAccessToken() const
{
    return Settings::getSingletonSettings()->currentUserData().getAccessToken();
}

QByteArray SettingOperator::getCurrentUserAuthorization() const
{
    return Settings::getSingletonSettings()->currentUserData().getAuthorization();
}

QByteArray SettingOperator::getCurrentUserClientSession() const
{
    return Settings::getSingletonSettings()->currentUserData().getClientSession();
}

QByteArray SettingOperator::getCurrentUserPassword() const
{
    return Settings::getSingletonSettings()->currentUserData().getPassword();
}

QByteArray SettingOperator::getCurrentUserSchoolId() const
{
    return Settings::getSingletonSettings()->currentUserData().getSchoolId();
}

QByteArray SettingOperator::getCurrentUserStudentId() const
{
    return Settings::getSingletonSettings()->currentUserData().getStudentId();
}

QByteArray SettingOperator::getCurrentUserUsername() const
{
    return Settings::getSingletonSettings()->currentUserData().getUsername();
}

bool SettingOperator::isCurrentUserEmpty() const
{
    return Settings::getSingletonSettings()->currentUserData().isEmpty();
}

bool SettingOperator::isCurrentUserValid() const
{
    return Settings::getSingletonSettings()->currentUserData().isValid();
}

bool SettingOperator::login(const QString &id, const QString &pw)
{
    auto newUserData(UserData::login(id.toUtf8(), pw.toUtf8()));
    if(newUserData.isValid())
    {
        Settings::getSingletonSettings()->userDataListPrepend(newUserData);
        return true;
    }
    else
    {
        return false;
    }
}

bool SettingOperator::currentUserRelogin()
{
    auto userDate(Settings::getSingletonSettings()->currentUserData());
    if(userDate.relogin())
    {
        Settings::getSingletonSettings()->userDataListRemoveFirst();
        Settings::getSingletonSettings()->userDataListPrepend(userDate);
        return true;
    }
    else
    {
        return false;
    }
}

QVariant SettingOperator::getUserDataList() const
{
    QStringList userDataList;
    for(const auto &i : Settings::getSingletonSettings()->getUserDataList())
    {
        userDataList.append(i.getDetailDataJsonObject()
                            .value(QStringLiteral("realName"))
                            .toString() +
                            QStringLiteral("  ") + i.getUsername());
    }
    if(userDataList.isEmpty())
    {
        userDataList.append(QStringLiteral("公共账号"));
    }
    return QVariant::fromValue(userDataList);
}

QVariant SettingOperator::getAnimeImageNameList() const
{
    QStringList animeImageNameList;
    for(const auto &i : Settings::getSingletonSettings()->getAnimeImageUrlList())
    {
        animeImageNameList.append(i.first);
    }
    return QVariant::fromValue(animeImageNameList);
}

QString SettingOperator::getVersion() const
{
    return QStringLiteral(APP_VERSION);
}

QString SettingOperator::getAnimeImageUrl(qsizetype index) const
{
    return Settings::getSingletonSettings()->getAnimeImageUrlList().at(index).second;
}

qsizetype SettingOperator::getCurrentAnimeImageNameIndex() const
{
    for(auto i(0); i < Settings::getSingletonSettings()->getAnimeImageUrlList().size(); ++i)
    {
        if(Settings::getSingletonSettings()->getAnimeImageUrlList().at(i).second == Settings::getSingletonSettings()->getAnimeImageUrl())
            return i;
    }
    return -1;
}

void SettingOperator::userDataListToFirst(qsizetype index)
{
    Settings::getSingletonSettings()->userDataListToFirst(index);
}

void SettingOperator::deleteTemplateFile()
{
    QFile::remove(Global::dataPath().append(QStringLiteral("/templateList_undefined")));
    Global::deleteDir(Global::dataPath().append(QStringLiteral("/TemplateFile")));
    QDir().mkdir(Global::dataPath().append(QStringLiteral("/TemplateFile")));
    MultipleSubjectsTemplateListModelListSingleton::getMultipleSubjectsTemplateListModelList()->at(MultipleSubjectsTemplateListModelList::Undefined)->clear();
}

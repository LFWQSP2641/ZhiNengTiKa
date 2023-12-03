#include "SettingOperator.h"
#include "MultipleSubjectsTemplateListModelListSingleton.h"
#include "../StaticClass/Global.h"
#include "../Singleton/Settings.h"

SettingOperator::SettingOperator(QObject *parent)
    : QObject{parent}
{

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

void SettingOperator::deleteTemplateFile()
{
    QFile::remove(Global::dataPath().append(QStringLiteral("/templateList_UserHistory")));
    Global::deleteDir(Global::dataPath().append(QStringLiteral("/TemplateFile")));
    QDir().mkdir(Global::dataPath().append(QStringLiteral("/TemplateFile")));
    MultipleSubjectsTemplateListModelListSingleton::getMultipleSubjectsTemplateListModelList()->at(MultipleSubjectsTemplateListModelList::UserHistory)->clear();
}

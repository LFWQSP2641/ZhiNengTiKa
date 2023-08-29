#include "SettingOperator.h"
#include "MultipleSubjectsTemplateListModelListSingleton.h"
#include "../StaticClass/Global.h"
#include "../Singleton/Settings.h"

SettingOperator::SettingOperator(QObject *parent)
    : QObject{parent}
{

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

QStringListModel *SettingOperator::getUserDataListModel()
{
    if(userDataListModel != nullptr)
    {
        userDataListModel->deleteLater();
    }
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
    userDataListModel = new QStringListModel(userDataList);
    return userDataListModel;
}

QString SettingOperator::getVersion()
{
    return QStringLiteral(APP_VERSION);
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

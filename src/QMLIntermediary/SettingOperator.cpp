#include "SettingOperator.h"
#include "../StaticClass/Setting.h"

SettingOperator::SettingOperator(QObject *parent)
    : QObject{parent}
{

}

bool SettingOperator::login(const QString &id, const QString &pw)
{
    auto newUserData(UserData::login(id.toUtf8(), pw.toUtf8()));
    if(newUserData.isEmpty())
    {
        return false;
    }
    else
    {
        Setting::userDataList.prepend(newUserData);
        return true;
    }
}

bool SettingOperator::logined()
{
    return Setting::logined();
}

QStringListModel *SettingOperator::getUserDataListModel()
{
    if(userDataListModel != nullptr)
    {
        userDataListModel->deleteLater();
    }
    QStringList userDataList;
    for(const auto &i : Setting::userDataList)
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

void SettingOperator::save()
{
    Setting::saveToFile();
}

void SettingOperator::logout()
{
    if(Setting::logined())
    {
        Setting::userDataList.removeFirst();
    }
}

void SettingOperator::userDataListToFirst(qsizetype index)
{
    Setting::userDataList.toFirst(index);
}


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

QString SettingOperator::getStyle()
{
    return Setting::qmlStyle;
}

void SettingOperator::setStyle(const QString &style)
{
    Setting::qmlStyle = style;
}

QString SettingOperator::getFont()
{
    return Setting::font;
}

void SettingOperator::setFont(const QString &font)
{
    Setting::font = font;
}

bool SettingOperator::getListLatestTemplatePreferentially()
{
    return Setting::listLatestTemplatePreferentially;
}

void SettingOperator::setListLatestTemplatePreferentially(bool state)
{
    Setting::listLatestTemplatePreferentially = state;
}

bool SettingOperator::getCompressQRCodeImage()
{
    return Setting::compressQRCodeImage;
}

void SettingOperator::setCompressQRCodeImage(bool state)
{
    Setting::compressQRCodeImage = state;
}

QString SettingOperator::getVersion()
{
    return QStringLiteral(APP_VERSION);
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


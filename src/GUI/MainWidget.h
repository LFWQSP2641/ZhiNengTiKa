#pragma once

#include "NavigationBarTabWidget.h"

class SearchWidget;
class SettingWidget;

#ifdef Q_OS_ANDROID
class TemplateDetailWidget;
#endif // Q_OS_ANDROID

class MainWidget : public NavigationBarTabWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);


#ifdef Q_OS_ANDROID
    enum TabIndex
    {
        SearchWidgetIndex = 0,
        TemplateDetailWidgetIndex = 1,
        SettingWidgetIndex = 2
    };
#else // Q_OS_ANDROID
    enum TabIndex
    {
        SearchWidgetIndex = 0,
        SettingWidgetIndex = 1
    };
#endif // Q_OS_ANDROID


protected:
#ifdef Q_OS_ANDROID
    TemplateDetailWidget *templateDetailWidget;
#endif // Q_OS_ANDROID

    SearchWidget *searchWidget;
    SettingWidget *settingWidget;

    void closeEvent(QCloseEvent *event) override;
};

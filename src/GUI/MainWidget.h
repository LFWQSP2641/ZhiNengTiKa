#pragma once

#include "NavigationBarTabWidget.h"

class SearchWidget;
class SettingWidget;

class MainWidget : public NavigationBarTabWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    SearchWidget *searchWidget;
    SettingWidget *settingWidget;

    void closeEvent(QCloseEvent *event) override;
#ifdef Q_OS_ANDROID
    void resizeEvent(QResizeEvent *event) override;
#endif

private slots:
};

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "NavigationBarTabWidget.h"

class SelectWidget;
class SettingWidget;

class MainWidget : public NavigationBarTabWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);

    enum TabIndex
    {
        SelectWidgetIndex = 0,
        SettingWidgetIndex = 1
    };

protected:
    SelectWidget *searchWidget;
    SettingWidget *settingWidget;

    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWIDGET_H

#include "../Singleton/Settings.h"
#include "MainWidget.h"
#include "SettingWidget.h"
#include "SelectWidget.h"

MainWidget::MainWidget(QWidget *parent)
    : NavigationBarTabWidget{parent}
{
    settingWidget = new SettingWidget(this);
    searchWidget = new SelectWidget(this);

    this->addTab(searchWidget, QStringLiteral("题卡"));
    this->addTabWithScrollArea(settingWidget, QStringLiteral("设置"));

    connect(this, &QTabWidget::currentChanged, [this](int index)
    {
        Settings::getSingletonSettings()->saveToFile();
        switch (index)
        {
        case TabIndex::SettingWidgetIndex:
            settingWidget->refreshTempSize();
            break;
        default:
            break;
        }
    });
}

void MainWidget::closeEvent(QCloseEvent *event)
{
    Settings::getSingletonSettings()->saveToFile();
    NavigationBarTabWidget::closeEvent(event);
    event->accept();
}

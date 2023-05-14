#include "../StaticClass/Setting.h"
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
    Setting::saveToFile();
    NavigationBarTabWidget::closeEvent(event);
    event->accept();
}

#include "MainWidget.h"

#include "../Singleton/Settings.h"
#include "SelectWidget.h"
#include "SettingWidget.h"

MainWidget::MainWidget(QWidget *parent)
    : NavigationBarTabWidget{ parent },
      selectWidget(new SelectWidget(this)),
      settingWidget(new SettingWidget(this))
{
    this->addTab(selectWidget, QStringLiteral("题卡"));
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
                } });
}

void MainWidget::closeEvent(QCloseEvent *event)
{
    Settings::getSingletonSettings()->saveToFile();
    NavigationBarTabWidget::closeEvent(event);
    event->accept();
}

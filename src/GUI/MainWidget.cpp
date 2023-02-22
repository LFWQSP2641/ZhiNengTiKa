#include "../StaticClass/Setting.h"
#include "MainWidget.h"
#include "SettingWidget.h"
#include "SearchWidget.h"
#include "src/StaticClass/Global.h"

MainWidget::MainWidget(QWidget *parent)
    : NavigationBarTabWidget{parent}
{
    settingWidget = new SettingWidget(this);
    searchWidget = new SearchWidget(this);

#ifdef Q_OS_ANDROID
    this->setFixedSize(this->size());
#endif

    this->addTab(searchWidget, QStringLiteral("题卡"));
    this->addTabWithScrollArea(settingWidget, QStringLiteral("设置"));

    connect(this, &QTabWidget::currentChanged, [this](int index)
    {
        switch (index)
        {
        case 1:
            settingWidget->refreshTempSize();
            break;
        default:
            break;
        }
    });
}

MainWidget::~MainWidget()
{
}

void MainWidget::closeEvent(QCloseEvent *event)
{
    Setting::saveToFile();
    event->accept();
}

#ifdef Q_OS_ANDROID
void MainWidget::resizeEvent(QResizeEvent *event)
{
    Global::mainWidgetSize = this->size();
    event->accept();
}
#endif

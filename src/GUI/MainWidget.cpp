#include "../StaticClass/Setting.h"
#include "MainWidget.h"
#include "SettingWidget.h"
#include "SelectWidget.h"

#ifdef Q_OS_ANDROID
#include "../GUI/TemplateDetailWidget.h"
#endif // Q_OS_ANDROID

MainWidget::MainWidget(QWidget *parent)
    : NavigationBarTabWidget{parent}
{
    settingWidget = new SettingWidget(this);
    searchWidget = new SelectWidget(this);

    this->addTab(searchWidget, QStringLiteral("题卡"));

#ifdef Q_OS_ANDROID
    templateDetailWidget = new TemplateDetailWidget(this);
    this->addTab(templateDetailWidget, QStringLiteral("解析"));

    connect(searchWidget, &SelectWidget::searchFinished, templateDetailWidget, &TemplateDetailWidget::setAnalysisWebRawData);
    connect(searchWidget, &SelectWidget::searchFinished, [this]
    {
        this->setCurrentIndex(TabIndex::TemplateDetailWidgetIndex);
        this->templateDetailWidget->setCurrentIndex(TemplateDetailWidget::TabIndex::AnswerAndAnalysisWidgetIndex);
    });
#endif // Q_OS_ANDROID

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

#include "../StaticClass/Setting.h"
#include "MainWidget.h"
#include "SettingWidget.h"
#include "SearchWidget.h"

#ifdef Q_OS_ANDROID
#include "../GUI/TemplateDetailWidget.h"
#endif // Q_OS_ANDROID

MainWidget::MainWidget(QWidget *parent)
    : NavigationBarTabWidget{parent}
{
    settingWidget = new SettingWidget(this);
    searchWidget = new SearchWidget(this);

    this->addTab(searchWidget, QStringLiteral("题卡"));

#ifdef Q_OS_ANDROID
    templateDetailWidget = new TemplateDetailWidget(this);
    this->addTab(templateDetailWidget, QStringLiteral("解析"));

    connect(searchWidget, &SearchWidget::searchFinished, templateDetailWidget, &TemplateDetailWidget::setAnalysisWebRawData);
    connect(searchWidget, &SearchWidget::searchFinished, [this]
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
    event->accept();
}

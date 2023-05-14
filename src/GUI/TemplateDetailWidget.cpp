#include "TemplateDetailWidget.h"
#include "AnswerAndAnalysisWidget.h"
#include "QuestionWidget.h"
#include "UploadWidget.h"
#include "../StaticClass/Setting.h"

TemplateDetailWidget::TemplateDetailWidget(QSharedPointer<TemplateAnalysis> templateAnalysis, QWidget *parent)
    : NavigationBarTabWidget{parent}
{
    answerAndAnalysisWidget = new AnswerAndAnalysisWidget(templateAnalysis);
    questionWidget = new QuestionWidget(templateAnalysis);
    uploadWidget = new UploadWidget(templateAnalysis);

    this->addTab(answerAndAnalysisWidget, QStringLiteral("答案及解析"));
    this->addTab(questionWidget, QStringLiteral("原题"));
    this->addTabWithScrollArea(uploadWidget, "上传");
}

void TemplateDetailWidget::setTemplateAnalysis(QSharedPointer<TemplateAnalysis> templateAnalysis)
{
    answerAndAnalysisWidget->setTemplateAnalysis(templateAnalysis);
    questionWidget->setTemplateAnalysis(templateAnalysis);
    uploadWidget->setTemplateAnalysis(templateAnalysis);
}

void TemplateDetailWidget::showEvent(QShowEvent *event)
{
    NavigationBarTabWidget::showEvent(event);
    this->setTabVisible(TabIndex::UploadWidgetIndex, Setting::logined());
    event->accept();
}

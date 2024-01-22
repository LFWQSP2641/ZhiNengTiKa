#include "TemplateDetailWidget.h"
#include "AnswerAndAnalysisWidget.h"
#include "QuestionWidget.h"
#include "UploadWidget.h"
#include "../Singleton/Settings.h"

TemplateDetailWidget::TemplateDetailWidget(const TemplateAnalysis &templateAnalysis, QWidget *parent)
    : NavigationBarTabWidget{parent}
{
    answerAndAnalysisWidget = new AnswerAndAnalysisWidget(templateAnalysis);
    questionWidget = new QuestionWidget(templateAnalysis);
    uploadWidget = new UploadWidget(templateAnalysis);

    this->addTab(answerAndAnalysisWidget, QStringLiteral("答案及解析"));
    this->addTab(questionWidget, QStringLiteral("原题"));
    this->addTabWithScrollArea(uploadWidget, "上传");
}

void TemplateDetailWidget::setTemplateAnalysis(const TemplateAnalysis &templateAnalysis)
{
    answerAndAnalysisWidget->setTemplateAnalysis(templateAnalysis);
    questionWidget->setTemplateAnalysis(templateAnalysis);
    uploadWidget->setTemplateAnalysis(templateAnalysis);
}

void TemplateDetailWidget::showEvent(QShowEvent *event)
{
    NavigationBarTabWidget::showEvent(event);
    this->setTabVisible(TabIndex::UploadWidgetIndex, Settings::getSingletonSettings()->getAccountManager()->isLoggedin());
    event->accept();
}

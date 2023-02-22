#include "TemplateDetailWidget.h"
#include "AnswerAndAnalysisWidget.h"
#include "QuestionWidget.h"
#include "UploadWidget.h"

TemplateDetailWidget::TemplateDetailWidget(const AnalysisWebRawData &analysisWebRawData, QWidget *parent)
    : NavigationBarTabWidget{parent}
{
    answerAndAnalysisWidget = new AnswerAndAnalysisWidget(analysisWebRawData);
    questionWidget = new QuestionWidget(analysisWebRawData);
    uploadWidget = new UploadWidget(analysisWebRawData);
    QTimer::singleShot(3000, [this]
    {
        this->addTab(answerAndAnalysisWidget, QStringLiteral("答案及解析"));
        this->addTab(questionWidget, QStringLiteral("原题"));
    });
    QTimer::singleShot(2000, [this]
    {
        this->addTabWithScrollArea(uploadWidget, "上传");
    });
#ifndef Q_OS_ANDROID
    this->resize(360, 720);
#endif
}

void TemplateDetailWidget::setAnalysisWebRawData(const AnalysisWebRawData &analysisWebRawData)
{
    answerAndAnalysisWidget->setAnalysisWebRawData(analysisWebRawData);
    questionWidget->setAnalysisWebRawData(analysisWebRawData);
    uploadWidget->setAnalysisWebRawData(analysisWebRawData);
}

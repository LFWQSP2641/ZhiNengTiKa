#include "QuestionWidget.h"
#include "../Logic/AnalysisWebRawData.h"

QuestionWidget::QuestionWidget(const AnalysisWebRawData &analysisWebRawData, QWidget *parent)
    : WebViewWidget(analysisWebRawData, parent)
{
    showCurrentAnswerButton = new QPushButton(QStringLiteral("显示答案"), this);
    mainLayout->addWidget(showCurrentAnswerButton, 0, 0, 1, 1);
    connect(showCurrentAnswerButton, &QPushButton::clicked, this, &QuestionWidget::showCurrentAnswer);
}

QString QuestionWidget::getAnalyzedHtml(const qsizetype index)
{
    return analysisWebRawData.getQuestionHtml(index);
}

void QuestionWidget::showCurrentAnswer()
{
    webView->setHtml(getAnalyzedHtml(currentPageIndex) + analysisWebRawData.getAnswerAndAnalysisHtml(currentPageIndex));
}

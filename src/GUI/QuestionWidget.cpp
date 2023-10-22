#include "QuestionWidget.h"

QuestionWidget::QuestionWidget(TemplateAnalysis *templateAnalysis, QWidget *parent)
    : WebViewWidget(templateAnalysis, parent)
{
    showCurrentAnswerButton = new QPushButton(QStringLiteral("显示答案"), this);
    mainLayout->addWidget(showCurrentAnswerButton, 0, 0, 1, 1);
    connect(showCurrentAnswerButton, &QPushButton::clicked, this, &QuestionWidget::showCurrentAnswer);
}

QString QuestionWidget::getAnalyzedHtml(const qsizetype index)
{
    return templateAnalysis->getQuestionHtml(index);
}

void QuestionWidget::showCurrentAnswer()
{
    webView->setHtml(getAnalyzedHtml(currentPageIndex) + templateAnalysis->getAnswerAndAnalysisHtml(currentPageIndex));
}

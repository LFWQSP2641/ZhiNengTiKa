#include "AnswerAndAnalysisWidget.h"

AnswerAndAnalysisWidget::AnswerAndAnalysisWidget(QSharedPointer<TemplateAnalysis> templateAnalysis, QWidget *parent)
    : WebViewWidget(templateAnalysis, parent)
{
    onlyAnswerCheckBox = new QCheckBox(QStringLiteral("Only Answer"), this);
    mainLayout->addWidget(onlyAnswerCheckBox, 0, 0, 1, 1);

    connect(this->onlyAnswerCheckBox, &QCheckBox::clicked, this, &AnswerAndAnalysisWidget::onlyAnswerCheckBoxClicked);
}

QString AnswerAndAnalysisWidget::getAnalyzedHtml(const qsizetype index)
{
    if(!this->templateAnalysisPointer->isValid())
    {
        return QString();
    }
    if(onlyAnswerCheckBox->isChecked())
    {
        return this->templateAnalysisPointer->getAnswerHtml(index);
    }
    else
    {
        return this->templateAnalysisPointer->getAnswerAndAnalysisHtml(index);
    }
}

void AnswerAndAnalysisWidget::onlyAnswerCheckBoxClicked([[maybe_unused]]bool checked)
{
    webView->setHtml(getAnalyzedHtml(currentPageIndex));
}

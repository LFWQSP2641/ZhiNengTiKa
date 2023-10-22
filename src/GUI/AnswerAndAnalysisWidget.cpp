#include "AnswerAndAnalysisWidget.h"

AnswerAndAnalysisWidget::AnswerAndAnalysisWidget(TemplateAnalysis *templateAnalysis, QWidget *parent)
    : WebViewWidget(templateAnalysis, parent)
{
    onlyAnswerCheckBox = new QCheckBox(QStringLiteral("Only Answer"), this);
    mainLayout->addWidget(onlyAnswerCheckBox, 0, 0, 1, 1);

    connect(this->onlyAnswerCheckBox, &QCheckBox::clicked, this, &AnswerAndAnalysisWidget::onlyAnswerCheckBoxClicked);
}

QString AnswerAndAnalysisWidget::getAnalyzedHtml(const qsizetype index)
{
    if(!this->templateAnalysis->getValid())
    {
        return QString();
    }
    if(onlyAnswerCheckBox->isChecked())
    {
        return this->templateAnalysis->getAnswerHtml(index);
    }
    else
    {
        return this->templateAnalysis->getAnswerAndAnalysisHtml(index);
    }
}

void AnswerAndAnalysisWidget::onlyAnswerCheckBoxClicked([[maybe_unused]]bool checked)
{
    webView->setHtml(getAnalyzedHtml(currentPageIndex));
}

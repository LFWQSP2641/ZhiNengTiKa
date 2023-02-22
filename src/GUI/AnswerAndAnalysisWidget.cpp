#include "AnswerAndAnalysisWidget.h"
#include "../Logic/AnalysisWebRawData.h"

AnswerAndAnalysisWidget::AnswerAndAnalysisWidget(const AnalysisWebRawData &analysisWebRawData, QWidget *parent)
    : WebViewWidget(analysisWebRawData, parent)
{
    onlyAnswerCheckBox = new QCheckBox(QStringLiteral("Only Answer"), this);
    mainLayout->addWidget(onlyAnswerCheckBox, 0, 0, 1, 1);

    connect(this->onlyAnswerCheckBox, &QCheckBox::clicked, this, &AnswerAndAnalysisWidget::onlyAnswerCheckBoxClicked);
}

QString AnswerAndAnalysisWidget::getAnalyzedHtml(const qsizetype index)
{
    if(onlyAnswerCheckBox->isChecked())
    {
        return analysisWebRawData.getAnswerHtml(index);
    }
    else
    {
        return analysisWebRawData.getAnswerAndAnalysisHtml(index);
    }
}

void AnswerAndAnalysisWidget::onlyAnswerCheckBoxClicked([[maybe_unused]]bool checked)
{
    webView->setHtml(getAnalyzedHtml(currentPageIndex));
}

#ifndef ANSWERANDANALYSISWIDGET_H
#define ANSWERANDANALYSISWIDGET_H

#include "WebViewWidget.h"

class AnswerAndAnalysisWidget : public WebViewWidget
{
    Q_OBJECT
public:
    explicit AnswerAndAnalysisWidget(QSharedPointer<TemplateAnalysis> templateAnalysis = QSharedPointer<TemplateAnalysis>(new TemplateAnalysis), QWidget *parent = nullptr);

private:
    QCheckBox *onlyAnswerCheckBox;

public slots:
    QString getAnalyzedHtml(const qsizetype index = -1) override;

private slots:
    void onlyAnswerCheckBoxClicked(bool checked);
};

#endif // ANSWERANDANALYSISWIDGET_H

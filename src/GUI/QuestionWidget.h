#ifndef QUESTIONWIDGET_H
#define QUESTIONWIDGET_H

#include "WebViewWidget.h"

class QuestionWidget : public WebViewWidget
{
    Q_OBJECT
public:
    explicit QuestionWidget(const TemplateAnalysis &templateAnalysis, QWidget *parent = nullptr);

private:
    QPushButton *showCurrentAnswerButton;

public slots:
    QString getAnalyzedHtml(const qsizetype index = -1) override;

private slots:
    void showCurrentAnswer();
};

#endif // QUESTIONWIDGET_H

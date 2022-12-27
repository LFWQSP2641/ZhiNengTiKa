#ifndef QUESTIONWIDGET_H
#define QUESTIONWIDGET_H

#include "WebViewWidget.h"
class AnalysisWebRawData;

class QuestionWidget : public WebViewWidget
{
    Q_OBJECT
public:
    explicit QuestionWidget(AnalysisWebRawData &analysisWebRawData, QWidget *parent = nullptr);

private:
    QPushButton *showCurrentAnswerButton;

public slots:
    QString getHtml(const qsizetype index = -1) override;

private slots:
    void showCurrentAnswer();
};

#endif // QUESTIONWIDGET_H

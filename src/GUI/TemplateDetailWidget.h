#ifndef TEMPLATEDETAILWIDGET_H
#define TEMPLATEDETAILWIDGET_H

#include "NavigationBarTabWidget.h"
#include "../Logic/TemplateAnalysis.h"

class AnswerAndAnalysisWidget;
class QuestionWidget;
class UploadWidget;

class TemplateDetailWidget : public NavigationBarTabWidget
{
    Q_OBJECT
public:
    explicit TemplateDetailWidget(TemplateAnalysis *templateAnalysis, QWidget *parent = nullptr);

    enum TabIndex
    {
        AnswerAndAnalysisWidgetIndex = 0,
        QuestionWidgetIndex = 1,
        UploadWidgetIndex = 2
    };

public slots:
    void setTemplateAnalysis(TemplateAnalysis *templateAnalysis);

protected:

    AnswerAndAnalysisWidget *answerAndAnalysisWidget;
    QuestionWidget *questionWidget;
    UploadWidget *uploadWidget;

    void showEvent(QShowEvent *event) override;
};

#endif // TEMPLATEDETAILWIDGET_H

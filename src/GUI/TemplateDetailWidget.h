#pragma once

#include "NavigationBarTabWidget.h"
#include "../Logic/AnalysisWebRawData.h"

class AnalysisWebRawData;
class AnswerAndAnalysisWidget;
class QuestionWidget;
class UploadWidget;

class TemplateDetailWidget : public NavigationBarTabWidget
{
    Q_OBJECT
public:
    explicit TemplateDetailWidget(QWidget *parent = nullptr);
    explicit TemplateDetailWidget(const AnalysisWebRawData &analysisWebRawData = AnalysisWebRawData(), QWidget *parent = nullptr);

    enum TabIndex
    {
        AnswerAndAnalysisWidgetIndex = 0,
        QuestionWidgetIndex = 1,
        UploadWidgetIndex = 2
    };

public slots:
    void setAnalysisWebRawData(const AnalysisWebRawData &analysisWebRawData);

protected:

    AnswerAndAnalysisWidget *answerAndAnalysisWidget;
    QuestionWidget *questionWidget;
    UploadWidget *uploadWidget;

    void showEvent(QShowEvent *event) override;
};


#pragma once

#include "NavigationBarTabWidget.h"

class AnalysisWebRawData;
class AnswerAndAnalysisWidget;
class QuestionWidget;
class UploadWidget;

class TemplateDetailWidget : public NavigationBarTabWidget
{
    Q_OBJECT
public:
    explicit TemplateDetailWidget(const AnalysisWebRawData &analysisWebRawData, QWidget *parent = nullptr);
    void setAnalysisWebRawData(const AnalysisWebRawData &analysisWebRawData);

protected:
    AnswerAndAnalysisWidget *answerAndAnalysisWidget;
    QuestionWidget *questionWidget;
    UploadWidget *uploadWidget;
};


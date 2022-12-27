#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "../Logic/AnalysisWebRawData.h"

class AnswerAndAnalysisWidget;
class SearchWidget;
class QuestionWidget;
class UploadWidget;
class SettingWidget;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    QGridLayout *mainLayout;
    QTabWidget *mainTabWidget;

    SearchWidget *searchWidget;
    AnswerAndAnalysisWidget *answerAndAnalysisWidget;
    QuestionWidget *questionWidget;
    UploadWidget *uploadWidget;
    SettingWidget *settingWidget;

    QWidget *addScrollArea(QWidget *Widget);

    AnalysisWebRawData analysisWebRawData;

    void closeEvent(QCloseEvent *event) override;
#ifdef Q_OS_ANDROID
    void resizeEvent(QResizeEvent *event) override;
#endif

private slots:
};
#endif // MAINWIDGET_H

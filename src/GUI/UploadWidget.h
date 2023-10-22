#ifndef UPLOADWIDGET_H
#define UPLOADWIDGET_H

#include "../Logic/TemplateAnalysis.h"

class UploadChildWidget;
class UserData;

class UploadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UploadWidget(TemplateAnalysis *templateAnalysis, QWidget *parent = nullptr);

protected:
    QVBoxLayout *mainLayout;
    QVBoxLayout *uploadChildWidgetLayout;
    QGroupBox *answerPrecedenceCheckBoxGroupBox;
    QRadioButton *rightAnswerPrecedenceCheckBox;
    QRadioButton *uploadAnswerPrecedenceCheckBox;
    QButtonGroup *buttonGroup;
    QPushButton *uploadButton;
    QPushButton *editRawDataButton;
    TemplateFetcher *fetcher;
    QMessageBox *handleTemplateRequestMessageBox;

    TemplateAnalysis *templateAnalysis;
    QList<UploadChildWidget*> uploadChildWidgetList;
    QList<QBitArray> rightAnswer;

    QJsonArray userAnswerList;

    bool templateAnalysisStateChanged = false;
    bool analysised = false;

    void showEvent(QShowEvent *event) override;

    QJsonObject getAnswerJsonObject();
    QJsonObject getAnswerJsonObject(const UserData &userData);

public slots:
    void setTemplateAnalysis(TemplateAnalysis *templateAnalysis);

protected slots:
    void analysis();
    void analysisUserAnswer(TemplateAnalysis *templateAnalysis, const QByteArray &rawData);
    bool upload();
    bool uploadData(const QByteArray &data);
    void getUserAnswer();
    void switchRightAnswerPrecedence();
    void switchUploadAnswerPrecedence();
    void editRawData();
    void setRightAnswer();
    void setUploadAnswer();
};

#endif // UPLOADWIDGET_H

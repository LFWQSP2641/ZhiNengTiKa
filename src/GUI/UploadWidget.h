#ifndef UPLOADWIDGET_H
#define UPLOADWIDGET_H

#include "../Logic/TemplateAnalysis.h"

class UploadChildWidget;
class UserData;

class UploadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UploadWidget(QSharedPointer<TemplateAnalysis> templateAnalysis = QSharedPointer<TemplateAnalysis>(new TemplateAnalysis), QWidget *parent = nullptr);

protected:
    QVBoxLayout *mainLayout;
    QVBoxLayout *uploadChildWidgetLayout;
    QGroupBox *answerPrecedenceCheckBoxGroupBox;
    QRadioButton *rightAnswerPrecedenceCheckBox;
    QRadioButton *uploadAnswerPrecedenceCheckBox;
    QButtonGroup *buttonGroup;
    QPushButton *uploadButton;
    QPushButton *editRawDataButton;

    QSharedPointer<TemplateAnalysis> templateAnalysisPointer;
    QList<UploadChildWidget*> uploadChildWidgetList;
    QList<QBitArray> rightAnswer;

    QJsonArray userAnswerList;

    bool templateAnalysisStateChanged = false;
    bool analysised = false;

    void showEvent(QShowEvent *event) override;

    QJsonObject getAnswerJsonObject();
    QJsonObject getAnswerJsonObject(const UserData &userData);

public slots:
    void setTemplateAnalysis(QSharedPointer<TemplateAnalysis> templateAnalysis);

protected slots:
    void analysis();
    bool upload();
    bool uploadData(const QByteArray &data);
    void getUserAnswer();
    void switchRightAnswerPrecedence();
    void switchUploadAnswerPrecedence();
    void editRawData();
};

#endif // UPLOADWIDGET_H

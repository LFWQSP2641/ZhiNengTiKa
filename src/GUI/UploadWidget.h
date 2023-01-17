#pragma once

class AnalysisWebRawData;
class UploadChildWidget;
class UserData;

class UploadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UploadWidget(AnalysisWebRawData &analysisWebRawData, QWidget *parent = nullptr);

protected:
    QVBoxLayout *mainLayout;
    QVBoxLayout *uploadChildWidgetLayout;
    QGroupBox *answerPrecedenceCheckBoxGroupBox;
    QRadioButton *rightAnswerPrecedenceCheckBox;
    QRadioButton *uploadAnswerPrecedenceCheckBox;
    QButtonGroup *buttonGroup;
    QPushButton *uploadButton;
    QPushButton *editRawDataButton;

    AnalysisWebRawData &analysisWebRawData;
    QList<UploadChildWidget*> uploadChildWidgetList;
    QList<QBitArray> rightAnswer;

    QJsonArray userAnswerList;

    bool analysisWebRawDataStateChanged = false;
    bool analysised = false;

    void showEvent(QShowEvent *event) override;

    QJsonObject getAnswerJsonObject();
    QJsonObject getAnswerJsonObject(const UserData &userData);

public slots:
    void analysis();

protected slots:
    bool upload();
    bool uploadData(const QByteArray &data);
    void getUserAnswer();
    void switchRightAnswerPrecedence();
    void switchuploadAnswerPrecedence();
    void editRawData();

signals:

};


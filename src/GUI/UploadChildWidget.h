#pragma once

#include "../Logic/AnswerDetailData.hpp"
class PixmapLabel;

class UploadChildWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UploadChildWidget(const AnswerDetailData &answerDetailData, QWidget *parent = nullptr);
    QJsonObject getJsonObject();
    bool isChoiceQuestions() const
    {
        return choiceQuestions;
    }

protected:
    QHBoxLayout *mainLayout;
    QHBoxLayout *answerLayout;
    QList<PixmapLabel*> pixmapLabelList;
    QButtonGroup *options = Q_NULLPTR;

    bool choiceQuestions;

    const AnswerDetailData answerDetailData;

public slots:
    void setPixmapFromNetwork(const QString &url);
    PixmapLabel *addPixmapLabel();
    void setChecked(const QBitArray &on);

signals:

};


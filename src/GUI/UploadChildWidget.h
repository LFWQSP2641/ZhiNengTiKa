#pragma once

#include "../Logic/AnswerDetailData.hpp"
class PixmapLabel;
class PlusSignLabel;

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
    PlusSignLabel *plusSignLabel = Q_NULLPTR;
    QButtonGroup *options = Q_NULLPTR;

    bool choiceQuestions;

    const AnswerDetailData answerDetailData;

public slots:
    void setPixmapFromNetwork(const QUrl &url);
    PixmapLabel *addPixmapLabelByPixmap(const QPixmap &pixmap);
    PixmapLabel *addPixmapLabelFromUrl(const QUrl &url);
    void setChecked(const QBitArray &on);
    void clearPixmapLabelList();

protected slots:
    PixmapLabel *addPixmapLabel();

signals:

};


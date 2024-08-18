#ifndef TEMPLATEANSWERDATA_H
#define TEMPLATEANSWERDATA_H

#include "src/ZhiNengTiKaCore_global.h"

class TemplateData;
class TemplateAnalysis;

class ZHINENGTIKACORE_EXPORT TemplateAnswerData
{
    Q_GADGET
    friend TemplateData;
    friend TemplateAnalysis;

public:
    explicit TemplateAnswerData(TemplateAnswerData *parentItem = nullptr);
    TemplateAnswerData(const QString &count, const QByteArray &questionId, const bool isAChoiceQuestion, int choiceQuestionCount = 0, QString &&answer = {}, TemplateAnswerData *parentItem = nullptr);
    ~TemplateAnswerData();

    QString getCount() const;

    QByteArray getQuestionId() const;

    bool isChoiceQuestion() const;

    bool isMultipleChoiceQuestion() const;

    QList<TemplateAnswerData *> getChildQuestionList() const;

    int getChoiceQuestionCount() const;

    QString getAnswer() const;

    int childCount() const;
    int row() const;
    TemplateAnswerData *child(int row) const;
    void addChild(TemplateAnswerData &&child);

    TemplateAnswerData *getParentItem() const;

protected:
    QString count;
    QByteArray questionId;
    bool choiceQuestion = false;
    bool multipleChoiceQuestion = false;
    int choiceQuestionCount = 0;
    QString answer;

    TemplateAnswerData *parentItem;
    QList<TemplateAnswerData *> childQuestionList;

private:
    Q_PROPERTY(QString count READ getCount CONSTANT FINAL)
    Q_PROPERTY(QByteArray questionId READ getQuestionId CONSTANT FINAL)
    Q_PROPERTY(bool choiceQuestion READ isChoiceQuestion CONSTANT FINAL)
    Q_PROPERTY(bool multipleChoiceQuestion READ isMultipleChoiceQuestion CONSTANT FINAL)
    Q_PROPERTY(QList<TemplateAnswerData *> childQuestionList READ getChildQuestionList CONSTANT FINAL)
    Q_PROPERTY(int choiceQuestionCount READ getChoiceQuestionCount CONSTANT FINAL)
    Q_PROPERTY(QString answer READ getAnswer CONSTANT FINAL)
    Q_PROPERTY(TemplateAnswerData *parentItem READ getParentItem CONSTANT FINAL)
};

#endif // TEMPLATEANSWERDATA_H

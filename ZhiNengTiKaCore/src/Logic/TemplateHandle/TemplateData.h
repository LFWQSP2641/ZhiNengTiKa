#ifndef TEMPLATEDATA_H
#define TEMPLATEDATA_H

#include "src/Logic/TemplateHandle/UploadModel/TemplateAnswerData.h"
#include "src/ZhiNengTiKaCore_global.h"

class TemplateAnalysis;

class ZHINENGTIKACORE_EXPORT TemplateData
{
    Q_GADGET
    friend TemplateAnalysis;

public:
    TemplateData();
    TemplateData(const TemplateData &other);
    ~TemplateData();

    TemplateData &operator=(const TemplateData &other);

    Q_INVOKABLE QString getCommonQuestionNumberStr() const;
    Q_INVOKABLE QString getAnswerAndAnalysisHtml() const;
    Q_INVOKABLE QString getAnswerHtml() const;
    Q_INVOKABLE QString getQuestionHtml() const;

    TemplateAnswerData getCountAndAnswer() const;

    QString getAnswerContent() const;
    QString getAnswerExplanation() const;
    QString getQuestionContent() const;
    QString getQuestionId() const;
    QStringList getOptions() const;
    QString getQuestionNumber() const;
    QString getGlobalQuestionNumber() const;
    QList<TemplateData *> getChildQuestionList() const;

    TemplateData *clone() const;
    QList<TemplateData *> cloneChildQuestionList() const;

    int childCount() const;
    int row() const;
    TemplateData *child(int row) const;
    void addChild(TemplateData *child);
    void addChildren(QList<TemplateData *> &&children);
    void addChild(const TemplateData &child);
    void addChildren(const QList<TemplateData> &children);

    TemplateData *getParentItem() const;

protected:
    QString answerContent;
    QString answerExplanation;
    QString questionContent;
    QString questionId;
    QStringList options;
    QString questionNumber;
    QString globalQuestionNumber;

    QList<TemplateData *> childQuestionList;
    TemplateData *parentItem = nullptr;

private:
    Q_PROPERTY(QString answerContent READ getAnswerContent CONSTANT FINAL)
    Q_PROPERTY(QString answerExplanation READ getAnswerExplanation CONSTANT FINAL)
    Q_PROPERTY(QString questionContent READ getQuestionContent CONSTANT FINAL)
    Q_PROPERTY(QString questionId READ getQuestionId CONSTANT FINAL)
    Q_PROPERTY(QStringList options READ getOptions CONSTANT FINAL)
    Q_PROPERTY(QString questionNumber READ getQuestionNumber CONSTANT FINAL)
    Q_PROPERTY(QString globalQuestionNumber READ getGlobalQuestionNumber CONSTANT FINAL)
    Q_PROPERTY(QList<TemplateData *> childQuestionList READ getChildQuestionList CONSTANT FINAL)
    Q_PROPERTY(TemplateData *parentItem READ getParentItem CONSTANT FINAL)
};

#endif // TEMPLATEDATA_H

#ifndef TEMPLATEANALYSIS_H
#define TEMPLATEANALYSIS_H

#include "TemplateSummary.h"
#include "AnswerDetailData.hpp"

class TemplateAnalysis : public TemplateSummary
{
    Q_GADGET
    friend class TemplateFetcher;
public:
    explicit TemplateAnalysis() = default;

    Q_INVOKABLE QString getAnswerAndAnalysisHtml(const qsizetype index = -1) const;
    Q_INVOKABLE QString getAnswerHtml(const qsizetype index = -1) const;
    Q_INVOKABLE QString getQuestionHtml(const qsizetype index = -1) const;
    QList<AnswerDetailData> getCountAndAnswer(const qsizetype index = -1) const;

    Q_INVOKABLE QStringList getQuestionsCountsStrList() const
    {
        return questionsCountsStrList;
    }

    Q_INVOKABLE QVariant getQuestionsCountsStrListModel() const
    {
        return QVariant::fromValue(getQuestionsCountsStrList());
    }
    bool getLocal() const;

    bool getNetwork() const;

    bool getValid() const;

    void analyze(const QByteArray &rawData);

protected:
    QStringList questionsCountsStrList;
    QJsonArray answerDataList;

    bool local = false;
    bool network = false;
    bool valid = false;

private:
    template<typename f>
    static void callFunc(const QJsonArray &jsonArray, const qsizetype index, f func);

    template<typename f1, typename f2>
    static void callFunc(const QJsonArray &jsonArray, const qsizetype index, f1 func1, f2 func2);
    Q_PROPERTY(bool local READ getLocal CONSTANT FINAL)
    Q_PROPERTY(bool network READ getNetwork CONSTANT FINAL)
    Q_PROPERTY(bool valid READ getValid CONSTANT FINAL)
};

#endif // TEMPLATEANALYSIS_H

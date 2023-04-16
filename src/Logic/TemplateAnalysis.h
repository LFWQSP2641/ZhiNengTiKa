#ifndef TEMPLATEANALYSIS_H
#define TEMPLATEANALYSIS_H

#include "TemplateRawData.h"
#include "AnswerDetailData.hpp"

class TemplateAnalysis : public TemplateRawData
{
public:
    TemplateAnalysis() = default;
    explicit TemplateAnalysis(const TemplateRawData &templateRawData);

    QString getAnswerAndAnalysisHtml(const qsizetype index = -1) const;
    QString getAnswerHtml(const qsizetype index = -1) const;
    QString getQuestionHtml(const qsizetype index = -1) const;
    QList<AnswerDetailData> getCountAndAnswer(const qsizetype index = -1) const;

    QStringList getQuestionsCountsStrList() const
    {
        return questionsCountsStrList;
    }

protected:
    QStringList questionsCountsStrList;
    QJsonArray answerDataList;

private:
    template<typename f>
    static void callFunc(const QJsonArray &jsonArray, const qsizetype index, f func);

    template<typename f1, typename f2>
    static void callFunc(const QJsonArray &jsonArray, const qsizetype index, f1 func1, f2 func2);
};

Q_DECLARE_METATYPE(TemplateAnalysis)

#endif // TEMPLATEANALYSIS_H

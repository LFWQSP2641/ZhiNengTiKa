#pragma once

#include "AnswerDetailData.hpp"

class AnalysisWebRawData
{
public:
    explicit AnalysisWebRawData(const QByteArray &webRawData, const QString &templateName, const QString &templateCode);

    QString getAnswerAndAnalysisHtml(const qsizetype index = -1) const;
    QString getAnswerHtml(const qsizetype index = -1) const;
    QString getQuestionHtml(const qsizetype index = -1) const;
    QList<AnswerDetailData> getCountAndAnswer(const qsizetype index = -1) const;

    QStringList getQuestionsCountsStrList() const
    {
        return questionsCountsStrList;
    }
    QString getTemplateName() const
    {
        return templateName;
    }
    QString getTemplateCode() const
    {
        return templateCode;
    }
    bool isEmpty() const
    {
        return answerDataList.isEmpty();
    }

protected:
    QString templateName;
    QString templateCode;
    QStringList questionsCountsStrList;

    QJsonArray answerDataList;

    void analysis(const QByteArray &webRawData);

    template<typename f>
    static void callFunc(const QJsonArray &jsonArray, const qsizetype index, f func);

    template<typename f1, typename f2>
    static void callFunc(const QJsonArray &jsonArray, const qsizetype index, f1 func1, f2 func2);
};


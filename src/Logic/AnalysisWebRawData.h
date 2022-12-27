#pragma once

#include "AnswerDetailData.hpp"

class AnalysisWebRawData : public QObject
{
    Q_OBJECT
public:
    explicit AnalysisWebRawData(QObject *parent = nullptr);

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

public slots:
    void analysis(const QByteArray &webRawData, const QString &templateName, const QString &templateCode);

protected:
    QString templateName;
    QString templateCode;
    QString answerSheetId;
    QStringList questionsCountsStrList;

    QJsonArray answerDataList;

    template<typename f>
    static void callFunc(const QJsonArray &jsonArray, const qsizetype index, f func);

    template<typename f1, typename f2>
    static void callFunc(const QJsonArray &jsonArray, const qsizetype index, f1 func1, f2 func2);

signals:
    void analysisFinished();
};


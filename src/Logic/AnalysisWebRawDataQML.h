#ifndef ANALYSISWEBRAWDATAQML_H
#define ANALYSISWEBRAWDATAQML_H

#include "AnswerDetailData.hpp"

class AnalysisWebRawDataQML: public QObject
{
    Q_OBJECT
public:
    AnalysisWebRawDataQML() = default;
    AnalysisWebRawDataQML(const QByteArray &webRawData, const QString &templateName, const QString &templateCode);

    Q_INVOKABLE void setValue(const QByteArray &webRawData, const QString &templateName, const QString &templateCode);
    Q_INVOKABLE void setValue(const AnalysisWebRawDataQML *other);

    Q_INVOKABLE QString getAnswerAndAnalysisHtml(const qsizetype index = -1) const;
    Q_INVOKABLE QString getAnswerHtml(const qsizetype index = -1) const;
    Q_INVOKABLE QString getQuestionHtml(const qsizetype index = -1) const;
    Q_INVOKABLE QList<AnswerDetailData> getCountAndAnswer(const qsizetype index = -1) const;

    Q_INVOKABLE QStringList getQuestionsCountsStrList() const
    {
        return questionsCountsStrList;
    }
    Q_INVOKABLE QStringListModel *getQuestionsCountsStrListModel()
    {
        if(this->newAnalysisWebRawDataQMLPointer != nullptr)
        {
            this->newAnalysisWebRawDataQMLPointer->deleteLater();
        }
        newAnalysisWebRawDataQMLPointer = new QStringListModel{questionsCountsStrList};
        return newAnalysisWebRawDataQMLPointer;
    }
    Q_INVOKABLE QString getTemplateName() const
    {
        return templateName;
    }
    Q_INVOKABLE QString getTemplateCode() const
    {
        return templateCode;
    }
    Q_INVOKABLE bool isEmpty() const
    {
        return answerDataList.isEmpty();
    }

protected:
    QString templateName;
    QString templateCode;
    QStringList questionsCountsStrList;

    QJsonArray answerDataList;

    QStringListModel *newAnalysisWebRawDataQMLPointer = nullptr;

    void analysis(const QByteArray &webRawData);

    template<typename f>
    static void callFunc(const QJsonArray &jsonArray, const qsizetype index, f func);

    template<typename f1, typename f2>
    static void callFunc(const QJsonArray &jsonArray, const qsizetype index, f1 func1, f2 func2);
};

#endif // ANALYSISWEBRAWDATAQML_H

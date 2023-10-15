#ifndef TEMPLATEANALYSIS_H
#define TEMPLATEANALYSIS_H

#include "TemplateSummary.h"
#include "AnswerDetailData.hpp"

class TemplateAnalysis : public TemplateSummary
{
    Q_OBJECT
    friend class TemplateFetcher;
public:
    explicit TemplateAnalysis(QObject *parent = nullptr)
        : TemplateSummary{parent} {}

    Q_INVOKABLE QString getAnswerAndAnalysisHtml(const qsizetype index = -1) const;
    Q_INVOKABLE QString getAnswerHtml(const qsizetype index = -1) const;
    Q_INVOKABLE QString getQuestionHtml(const qsizetype index = -1) const;
    QList<AnswerDetailData> getCountAndAnswer(const qsizetype index = -1) const;

    QStringList getQuestionsCountsStrList() const
    {
        return questionsCountsStrList;
    }

    Q_INVOKABLE QVariant getQuestionsCountsStrListModel() const
    {
        return QVariant::fromValue(getQuestionsCountsStrList());
    }

    bool getInternal() const;

    bool getExternal() const;

    bool getNetwork() const;

    bool getValid() const;

public slots:
    void analyze(const QByteArray &rawData);
    void deleteLater();

signals:

protected:
    QStringList questionsCountsStrList;
    QJsonArray answerDataList;

    bool internal = false;
    bool external = false;
    bool network = false;
    bool valid = false;

private:
    template<typename f>
    static void callFunc(const QJsonArray &jsonArray, const qsizetype index, f func);

    template<typename f1, typename f2>
    static void callFunc(const QJsonArray &jsonArray, const qsizetype index, f1 func1, f2 func2);
    Q_PROPERTY(bool internal READ getInternal CONSTANT FINAL)
    Q_PROPERTY(bool external READ getExternal CONSTANT FINAL)
    Q_PROPERTY(bool network READ getNetwork CONSTANT FINAL)
    Q_PROPERTY(bool valid READ getValid CONSTANT FINAL)
};

Q_DECLARE_METATYPE(TemplateAnalysis)

#endif // TEMPLATEANALYSIS_H

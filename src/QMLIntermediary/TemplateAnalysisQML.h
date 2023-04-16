#ifndef TEMPLATEANALYSISQML_H
#define TEMPLATEANALYSISQML_H

#include "../Logic/TemplateAnalysis.h"

class TemplateRawDataQML;

class TemplateAnalysisQML : public QObject
{
    Q_OBJECT
public:
    explicit TemplateAnalysisQML(QObject *parent = nullptr);
    explicit TemplateAnalysisQML(const TemplateRawData &templateRawData, QObject *parent = nullptr);
    explicit TemplateAnalysisQML(const TemplateRawDataQML &templateRawDataQML, QObject *parent = nullptr);

    Q_INVOKABLE void setValue(TemplateRawDataQML *templateRawDataQML);
    Q_INVOKABLE QStringListModel *getQuestionsCountsStrListModel()
    {
        if(tempQuestionsCountsStrListModelPointer != nullptr)
        {
            tempQuestionsCountsStrListModelPointer->deleteLater();
        }
        tempQuestionsCountsStrListModelPointer = new QStringListModel(templateAnalysis.getQuestionsCountsStrList());
        return tempQuestionsCountsStrListModelPointer;
    }

    // TemplateAnalysis function start
    Q_INVOKABLE QString getAnswerAndAnalysisHtml(const qsizetype index = -1) const
    {
        return templateAnalysis.getAnswerAndAnalysisHtml(index);
    }
    Q_INVOKABLE QString getAnswerHtml(const qsizetype index = -1) const
    {
        return templateAnalysis.getAnswerHtml(index);
    }
    Q_INVOKABLE QString getQuestionHtml(const qsizetype index = -1) const
    {
        return templateAnalysis.getQuestionHtml(index);
    }
    Q_INVOKABLE QList<AnswerDetailData> getCountAndAnswer(const qsizetype index = -1) const
    {
        return templateAnalysis.getCountAndAnswer(index);
    }
    Q_INVOKABLE QStringList getQuestionsCountsStrList() const
    {
        return templateAnalysis.getQuestionsCountsStrList();
    }
    // TemplateAnalysis function end

protected:
    TemplateAnalysis templateAnalysis;

    QStringListModel *tempQuestionsCountsStrListModelPointer = nullptr;
};

#endif // TEMPLATEANALYSISQML_H

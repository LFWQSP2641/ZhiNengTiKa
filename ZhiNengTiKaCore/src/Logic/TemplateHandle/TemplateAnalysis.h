#ifndef TEMPLATEANALYSIS_H
#define TEMPLATEANALYSIS_H

#include "TemplateSummary.h"
#include "src/Logic/TemplateHandle/TemplateData.h"
#include "src/Logic/TemplateHandle/TemplateModel/TemplateDataTreeModel.h"
#include "src/Logic/TemplateHandle/UploadModel/TemplateAnswerData.h"
#include "src/ZhiNengTiKaCore_global.h"

class ZHINENGTIKACORE_EXPORT TemplateAnalysis : public TemplateSummary
{
    Q_GADGET
    friend class TemplateFetcher;

public:
    explicit TemplateAnalysis();

    Q_INVOKABLE QString getAnswerAndAnalysisHtml(const qsizetype index = -1) const;
    Q_INVOKABLE QString getAnswerHtml(const qsizetype index = -1) const;
    Q_INVOKABLE QString getQuestionHtml(const qsizetype index = -1) const;
    Q_INVOKABLE TemplateAnswerData getCountAndAnswer(const qsizetype index = -1) const;

    Q_INVOKABLE TemplateDataTreeModel *getTemplateDataTreeModel() const;

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

    QList<QSharedPointer<TemplateData>> getTemplateDataList() const;

public slots:
    void analyze(const QByteArray &rawData);
    QSharedPointer<TemplateData> createTemplateData(const QJsonObject &object, const QString &globalQuestionNumber, const QString &questionNumber = {});

protected:
    QStringList questionsCountsStrList;
    QList<QSharedPointer<TemplateData>> templateDataList;

    QSharedPointer<TemplateDataTreeModel> templateDataTreeModel;

    bool local = false;
    bool network = false;
    bool valid = false;

private:
    Q_PROPERTY(bool local READ getLocal CONSTANT FINAL)
    Q_PROPERTY(bool network READ getNetwork CONSTANT FINAL)
    Q_PROPERTY(bool valid READ getValid CONSTANT FINAL)
};

Q_DECLARE_METATYPE(TemplateAnalysis)

#endif // TEMPLATEANALYSIS_H

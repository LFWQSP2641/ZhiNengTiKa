#ifndef TEMPLATEFETCHER_H
#define TEMPLATEFETCHER_H

#include "TemplateAnalysis.h"

class TemplateFetcher : public QObject
{
    Q_OBJECT
public:
    explicit TemplateFetcher(QObject *parent = nullptr);
public slots:
    void handleTemplateRequest(TemplateSummary *templateSummary);
    void handleTemplateRequestByCode(const QString &templateCode);
protected:
    TemplateAnalysis *templateAnalysisfromNetwork = nullptr;
protected slots:
    void onHandleTemplateReplyFinished();
signals:
    void obtainTemplateFromNetwork();
    void templateAnalysisReady(const TemplateAnalysis *templateAnalysis);
    void error(const QString &msg);
};

#endif // TEMPLATEFETCHER_H

#ifndef MULTIPLESUBJECTSTEMPLATELISTMODELLIST_H
#define MULTIPLESUBJECTSTEMPLATELISTMODELLIST_H

#include "TemplateListModel.h"

class MultipleSubjectsTemplateListModelList : public QObject
{
    Q_OBJECT
public:
    explicit MultipleSubjectsTemplateListModelList(QObject *parent = nullptr);
    ~MultipleSubjectsTemplateListModelList();
    enum Subjects
    {
        UserHistory = 0,
        Chinese = 1,
        Mathematics = 2,
        English = 3,
        Physics = 4,
        Chemistry = 5,
        Biography = 6
    };
    Q_INVOKABLE TemplateListModel *at(qsizetype index) const
    {
        return templateListModelList.at(index);
    }
public slots:
    void addNewTemplate(const TemplateSummary &templateSummary);
    void addNewTemplate(const QString &templateName, const QString &templateCode);

    void addTemplateList(const TemplateSummary &templateSummary);
    void addTemplateList(const QString &templateName, const QString &templateCode);

protected:
    QList<TemplateListModel*> templateListModelList;
};

#endif // MULTIPLESUBJECTSTEMPLATELISTMODELLIST_H

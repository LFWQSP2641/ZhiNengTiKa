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
    void writeUserHistoryToFile() const;
protected:
    QList<TemplateListModel*> templateListModelList;
protected slots:
    void importTemplateList(const QString &filePath, bool split = true);
};

#endif // MULTIPLESUBJECTSTEMPLATELISTMODELLIST_H

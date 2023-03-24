#ifndef MULTIPLESUBJECTSTEMPLATELISTMODELLIST_H
#define MULTIPLESUBJECTSTEMPLATELISTMODELLIST_H

#include "TemplateListModel.h"

class MultipleSubjectsTemplateListModelList : public QObject
{
//    Q_OBJECT
public:
    explicit MultipleSubjectsTemplateListModelList(QObject *parent = nullptr);
    ~MultipleSubjectsTemplateListModelList();
    enum Subjects
    {
        Chinese = 0,
        Mathematics = 1,
        English = 2,
        Physics = 3,
        Chemistry = 4,
        Biography = 5,
        Undefined = 6
    };
    Q_INVOKABLE TemplateListModel *at(qsizetype index) const
    {
        return templateListModelList.at(index);
    }
public slots:
    void addNewTemplate(QPair<QString, QString> templateInfo);

protected:
    QList<TemplateListModel*> templateListModelList;
};

#endif // MULTIPLESUBJECTSTEMPLATELISTMODELLIST_H

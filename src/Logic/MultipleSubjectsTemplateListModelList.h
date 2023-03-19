#ifndef MULTIPLESUBJECTSTEMPLATELISTMODELLIST_H
#define MULTIPLESUBJECTSTEMPLATELISTMODELLIST_H

#include "TemplateListModel.h"

class MultipleSubjectsTemplateListModelList : public QList<TemplateListModel*>
{
public:
    MultipleSubjectsTemplateListModelList();
    void addNewTemplate(QPair<QString, QString> templateInfo);
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
};

#endif // MULTIPLESUBJECTSTEMPLATELISTMODELLIST_H

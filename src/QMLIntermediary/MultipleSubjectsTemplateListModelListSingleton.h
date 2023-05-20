#ifndef MULTIPLESUBJECTSTEMPLATELISTMODELLISTSINGLETON_H
#define MULTIPLESUBJECTSTEMPLATELISTMODELLISTSINGLETON_H

#include "src/Logic/MultipleSubjectsTemplateListModelList.h"

class MultipleSubjectsTemplateListModelListSingleton
{
public:
    static void initOnce();
    static void resetMultipleSubjectsTemplateListModelList();
    static MultipleSubjectsTemplateListModelList *getMultipleSubjectsTemplateListModelList();
private:
    MultipleSubjectsTemplateListModelListSingleton() = default;
    static MultipleSubjectsTemplateListModelList *multipleSubjectsTemplateListModelList;
};

#endif // MULTIPLESUBJECTSTEMPLATELISTMODELLISTSINGLETON_H

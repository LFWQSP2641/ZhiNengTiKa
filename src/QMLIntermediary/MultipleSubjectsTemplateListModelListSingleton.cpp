#include "MultipleSubjectsTemplateListModelListSingleton.h"

MultipleSubjectsTemplateListModelList *MultipleSubjectsTemplateListModelListSingleton::multipleSubjectsTemplateListModelList;

void MultipleSubjectsTemplateListModelListSingleton::initOnce()
{
    MultipleSubjectsTemplateListModelListSingleton::multipleSubjectsTemplateListModelList = new MultipleSubjectsTemplateListModelList;
}

void MultipleSubjectsTemplateListModelListSingleton::resetMultipleSubjectsTemplateListModelList()
{
    MultipleSubjectsTemplateListModelListSingleton::multipleSubjectsTemplateListModelList->deleteLater();
    MultipleSubjectsTemplateListModelListSingleton::initOnce();
}

MultipleSubjectsTemplateListModelList *MultipleSubjectsTemplateListModelListSingleton::getMultipleSubjectsTemplateListModelList()
{
    return MultipleSubjectsTemplateListModelListSingleton::multipleSubjectsTemplateListModelList;
}

#ifndef MULTIPLESUBJECTSTEMPLATELISTVIEW_H
#define MULTIPLESUBJECTSTEMPLATELISTVIEW_H

#include "../Logic/MultipleSubjectsTemplateListModelList.h"
#include "../Logic/TemplateSummary.h"

class TemplateListView;

class MultipleSubjectsTemplateListView : public QWidget
{
    Q_OBJECT
public:
    explicit MultipleSubjectsTemplateListView(QWidget *parent = nullptr);
    void addNewTemplate(const TemplateSummary &templateSummary);

    MultipleSubjectsTemplateListModelList &getMultipleSubjectsTemplateListModelList();
    TemplateSummary getCurrentTemplateSummary();

protected:
    QVBoxLayout *mainLayout;
    QTabBar *multipleSubjectsTabBar;
    TemplateListView *templateListView;
    MultipleSubjectsTemplateListModelList multipleSubjectsTemplateListModelList;

signals:
    void templateNameClicked(const TemplateSummary &templateSummary);
};

#endif // MULTIPLESUBJECTSTEMPLATELISTVIEW_H

#ifndef MULTIPLESUBJECTSTEMPLATELISTVIEW_H
#define MULTIPLESUBJECTSTEMPLATELISTVIEW_H

#include "../Logic/MultipleSubjectsTemplateListModelList.h"

class TemplateListView;

class MultipleSubjectsTemplateListView : public QWidget
{
    Q_OBJECT
public:
    explicit MultipleSubjectsTemplateListView(QWidget *parent = nullptr);
    void addNewTemplate(QPair<QString, QString> templateInfo);

    MultipleSubjectsTemplateListModelList &getMultipleSubjectsTemplateListModelList();

protected:
    QVBoxLayout *mainLayout;
    QTabBar *multipleSubjectsTabBar;
    TemplateListView *templateListView;
    MultipleSubjectsTemplateListModelList multipleSubjectsTemplateListModelList;

signals:
    void templateNameClicked(const QString &templateCode);
};

#endif // MULTIPLESUBJECTSTEMPLATELISTVIEW_H

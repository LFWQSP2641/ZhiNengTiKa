#ifndef TEMPLATELISTVIEW_H
#define TEMPLATELISTVIEW_H

class TemplateListModel;
class TemplateSummary;

class TemplateListView : public QListView
{
    Q_OBJECT
public:
    explicit TemplateListView(QWidget *parent = nullptr);
    TemplateSummary *getCurrentTemplateSummary();
private:
    TemplateListModel *templateListModel = nullptr;
    void setModel(QAbstractItemModel *model) override
    {
        QListView::setModel(model);
    }
    TemplateSummary *getTemplateSummary(const QModelIndex &templateNameIndex);
public slots:
    void clear();
    void addNewTemplate(TemplateSummary *templateSummary);
    void setTemplateListModel(TemplateListModel *model);
protected slots:
    void emitTemplateSummary(const QModelIndex &templateNameIndex);
signals:
    void templateNameClicked(TemplateSummary *templateSummary);
};

#endif // TEMPLATELISTVIEW_H

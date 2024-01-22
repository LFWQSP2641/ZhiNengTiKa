#ifndef TEMPLATELISTVIEW_H
#define TEMPLATELISTVIEW_H

class TemplateListModel;
class TemplateSummary;

class TemplateListView : public QListView
{
    Q_OBJECT
public:
    explicit TemplateListView(QWidget *parent = nullptr);
    const TemplateSummary &getCurrentTemplateSummary();
private:
    TemplateListModel *templateListModel = nullptr;
    void setModel(QAbstractItemModel *model) override
    {
        QListView::setModel(model);
    }
    const TemplateSummary &getTemplateSummary(const QModelIndex &templateNameIndex);
public slots:
    void clear();
    void addNewTemplate(const TemplateSummary &templateSummary);
    void setTemplateListModel(TemplateListModel *model);
protected slots:
    void emitTemplateSummary(const QModelIndex &templateNameIndex);
signals:
    void templateNameClicked(const TemplateSummary &templateSummary);
};

#endif // TEMPLATELISTVIEW_H

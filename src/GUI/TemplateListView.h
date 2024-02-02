#ifndef TEMPLATELISTVIEW_H
#define TEMPLATELISTVIEW_H

class TemplateListModel;
class TemplateSummary;

class TemplateListView : public QListView
{
    Q_OBJECT
public:
    explicit TemplateListView(QWidget *parent = nullptr);
    TemplateSummary getCurrentTemplateSummary() const;
    TemplateSummary getTemplateSummary(const QModelIndex &templateNameIndex) const;
protected:
    TemplateListModel *templateListModel = nullptr;
    void setModel(QAbstractItemModel *model) override
    {
        QListView::setModel(model);
    }
public slots:
    void setTemplateListModel(TemplateListModel *model);
protected slots:
    void emitTemplateSummary(const QModelIndex &templateNameIndex);
signals:
    void templateNameClicked(const TemplateSummary &templateSummary);
};

#endif // TEMPLATELISTVIEW_H

#ifndef TEMPLATELISTVIEW_H
#define TEMPLATELISTVIEW_H

class TemplateListModel;

class TemplateListView : public QListView
{
    Q_OBJECT
public:
    explicit TemplateListView(QWidget *parent = nullptr);
    void setTemplateListModel(TemplateListModel *model);
private:
    TemplateListModel *templateListModel = nullptr;
    void setModel(QAbstractItemModel *model) override
    {
        QListView::setModel(model);
    }
protected slots:
    QString findTemplateCode(const QModelIndex &templateNameIndex);
signals:
    void templateNameClicked(const QString &templateCode);
};

#endif // TEMPLATELISTVIEW_H

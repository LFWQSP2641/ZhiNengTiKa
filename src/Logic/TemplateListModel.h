#ifndef TEMPLATELISTMODEL_H
#define TEMPLATELISTMODEL_H

class TemplateListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit TemplateListModel(const QList<QPair<QString, QString>> &templateList, QObject *parent = nullptr)
        : QAbstractListModel{parent}, templateList(templateList) {}
    explicit TemplateListModel(const QList<QString> &templateNameList, const QList<QString> &templateCodeList, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void addNewTemplate(QPair<QString, QString> templateInfo);

    QString getTemplateCode(const QModelIndex &index) const;
    bool hasTemplateName(const QString &templateName) const;
    bool hasTemplateCode(const QString &templateCode) const;
private:
    QList<QPair<QString, QString>> templateList;
};

#endif // TEMPLATELISTMODEL_H

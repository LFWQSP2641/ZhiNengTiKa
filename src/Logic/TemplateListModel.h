#ifndef TEMPLATELISTMODEL_H
#define TEMPLATELISTMODEL_H

#include "TemplateSummary.h"

class TemplateListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    using QAbstractListModel::QAbstractListModel;
    explicit TemplateListModel(const QList<TemplateSummary> &templateList, QObject *parent = nullptr)
        : QAbstractListModel{parent}, templateList(templateList) {}
    explicit TemplateListModel(const QList<QString> &templateNameList, const QList<QString> &templateCodeList, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE QString getTemplateCode(const QModelIndex &index) const;
    Q_INVOKABLE QString getTemplateCode(int index) const;
    Q_INVOKABLE bool hasTemplateName(const QString &templateName) const;
    Q_INVOKABLE bool hasTemplateCode(const QString &templateCode) const;
public slots:
    void addNewTemplate(const TemplateSummary &templateInfo);
    void clear();
private:
    QList<TemplateSummary> templateList;
};

#endif // TEMPLATELISTMODEL_H

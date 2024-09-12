#ifndef TEMPLATEDATATREEMODEL_H
#define TEMPLATEDATATREEMODEL_H

#include "src/Logic/TemplateHandle/TemplateData.h"
#include "src/ZhiNengTiKaCore_global.h"

class TemplateAnalysis;

class ZHINENGTIKACORE_EXPORT TemplateDataTreeModel : public QAbstractItemModel
{
    Q_OBJECT
    friend TemplateAnalysis;
    explicit TemplateDataTreeModel(QObject *parent = nullptr);

public:
    enum DataRoles
    {
        AnswerContent = Qt::UserRole + 1,
        AnswerExplanation,
        QuestionContent,
        QuestionId,
        Options,
        QuestionNumber,
        GlobalQuestionNumber
    };

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = {}) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;

protected:
    TemplateData rootItem;

    QHash<int, QByteArray> roleNames() const override;
};

#endif // TEMPLATEDATATREEMODEL_H

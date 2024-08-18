#ifndef TEMPLATEANSWERTREEMODEL_H
#define TEMPLATEANSWERTREEMODEL_H

#include "src/Logic/TemplateHandle/UploadModel/TemplateAnswerData.h"
#include "src/ZhiNengTiKaCore_global.h"

class ZHINENGTIKACORE_EXPORT TemplateAnswerTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TemplateAnswerTreeModel(QObject *parent = nullptr);
    TemplateAnswerTreeModel(TemplateAnswerData &&rootItem, QObject *parent = nullptr);

    void setRootItem(TemplateAnswerData &&rootItem);

    enum DataRoles
    {
        QuestionNumber = Qt::UserRole + 1,
        QuestionId,
        ChoiceQuestion,
        MultipleChoiceQuestion,
        ChoiceQuestionCount,
        Answer
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
    std::unique_ptr<TemplateAnswerData> rootItem;

    QHash<int, QByteArray> roleNames() const override;
};

#endif // TEMPLATEANSWERTREEMODEL_H

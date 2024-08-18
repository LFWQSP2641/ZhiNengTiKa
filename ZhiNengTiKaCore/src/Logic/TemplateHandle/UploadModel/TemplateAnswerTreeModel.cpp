#include "TemplateAnswerTreeModel.h"

TemplateAnswerTreeModel::TemplateAnswerTreeModel(QObject *parent)
    : QAbstractItemModel{ parent }
{
}

TemplateAnswerTreeModel::TemplateAnswerTreeModel(TemplateAnswerData &&rootItem, QObject *parent)
    : QAbstractItemModel{ parent }, rootItem(std::make_unique<TemplateAnswerData>(rootItem))
{
}

void TemplateAnswerTreeModel::setRootItem(TemplateAnswerData &&rootItem)
{
    this->rootItem = std::make_unique<TemplateAnswerData>(rootItem);
}

QVariant TemplateAnswerTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }
    int row = index.row();
    if (row < 0 || row >= rootItem->childCount())
    {
        return QVariant();
    }

    const auto *item = static_cast<const TemplateAnswerData *>(index.internalPointer());

    switch (role)
    {
    case QuestionNumber:
        return item->getCount();
        break;
    case QuestionId:
        return item->getQuestionId();
        break;
    case ChoiceQuestion:
        return item->isChoiceQuestion();
        break;
    case MultipleChoiceQuestion:
        return item->isMultipleChoiceQuestion();
        break;
    case ChoiceQuestionCount:
        return item->getChoiceQuestionCount();
        break;
    case Answer:
        return item->getAnswer();
        break;
    default:
        return QVariant();
        break;
    }
}

Qt::ItemFlags TemplateAnswerTreeModel::flags(const QModelIndex &index) const
{
    return index.isValid()
               ? QAbstractItemModel::flags(index)
               : Qt::ItemFlags(Qt::NoItemFlags);
}

QVariant TemplateAnswerTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }
    if (orientation == Qt::Horizontal)
    {
        return QStringLiteral("C %0").arg(section);
    }
    else
    {
        return QStringLiteral("R %0").arg(section);
    }
}

QModelIndex TemplateAnswerTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return {};

    TemplateAnswerData *parentItem = parent.isValid()
                                         ? static_cast<TemplateAnswerData *>(parent.internalPointer())
                                         : rootItem.get();

    if (auto *childItem = parentItem->child(row))
        return createIndex(row, column, childItem);
    return {};
}

QModelIndex TemplateAnswerTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return {};

    auto *childItem = static_cast<TemplateAnswerData *>(index.internalPointer());
    TemplateAnswerData *parentItem = childItem->getParentItem();

    return parentItem != rootItem.get()
               ? createIndex(parentItem->row(), 0, parentItem)
               : QModelIndex{};
}

int TemplateAnswerTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    const TemplateAnswerData *parentItem = parent.isValid()
                                               ? static_cast<const TemplateAnswerData *>(parent.internalPointer())
                                               : rootItem.get();

    return parentItem->childCount();
}

int TemplateAnswerTreeModel::columnCount([[maybe_unused]] const QModelIndex &parent) const
{
    return 1;
}

QHash<int, QByteArray> TemplateAnswerTreeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[QuestionNumber] = "questionNumber";
    roles[QuestionId] = "questionId";
    roles[ChoiceQuestion] = "choiceQuestion";
    roles[MultipleChoiceQuestion] = "multipleChoiceQuestion";
    roles[ChoiceQuestionCount] = "choiceQuestionCount";
    roles[Answer] = "answer";
    return roles;
}

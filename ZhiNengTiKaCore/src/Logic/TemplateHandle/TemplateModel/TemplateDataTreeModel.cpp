#include "TemplateDataTreeModel.h"

TemplateDataTreeModel::TemplateDataTreeModel(QObject *parent)
    : QAbstractItemModel{ parent }
{
}

QVariant TemplateDataTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }
    int row = index.row();
    if (row < 0 || row >= rootItem.childCount())
    {
        return QVariant();
    }

    const auto *item = static_cast<const TemplateData *>(index.internalPointer());

    switch (role)
    {
    case AnswerContent:
        return item->getAnswerContent();
        break;
    case AnswerExplanation:
        return item->getAnswerExplanation();
        break;
    case QuestionContent:
        return item->getQuestionContent();
        break;
    case QuestionId:
        return item->getQuestionId();
        break;
    case Options:
        return item->getOptions();
        break;
    case QuestionNumber:
        return item->getQuestionNumber();
        break;
    case GlobalQuestionNumber:
        return item->getGlobalQuestionNumber();
        break;
    default:
        return QVariant();
        break;
    }
}

Qt::ItemFlags TemplateDataTreeModel::flags(const QModelIndex &index) const
{
    return index.isValid()
               ? QAbstractItemModel::flags(index)
               : Qt::ItemFlags(Qt::NoItemFlags);
}

QVariant TemplateDataTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
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

QModelIndex TemplateDataTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return {};

    const TemplateData *parentItem = parent.isValid()
                                         ? static_cast<TemplateData *>(parent.internalPointer())
                                         : &rootItem;

    if (auto *childItem = parentItem->child(row))
        return createIndex(row, column, childItem);
    return {};
}

QModelIndex TemplateDataTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return {};

    auto *childItem = static_cast<TemplateData *>(index.internalPointer());
    TemplateData *parentItem = childItem->getParentItem();

    return parentItem != &rootItem
               ? createIndex(parentItem->row(), 0, parentItem)
               : QModelIndex{};
}

int TemplateDataTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    const TemplateData *parentItem = parent.isValid()
                                         ? static_cast<const TemplateData *>(parent.internalPointer())
                                         : &rootItem;

    return parentItem->childCount();
}

int TemplateDataTreeModel::columnCount([[maybe_unused]] const QModelIndex &parent) const
{
    return 1;
}

QHash<int, QByteArray> TemplateDataTreeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[AnswerContent] = "answerContent";
    roles[AnswerExplanation] = "answerExplanation";
    roles[QuestionContent] = "questionContent";
    roles[QuestionId] = "questionId";
    roles[Options] = "options";
    roles[QuestionNumber] = "questionNumber";
    roles[GlobalQuestionNumber] = "globalQuestionNumber";
    return roles;
}

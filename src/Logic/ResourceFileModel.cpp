#include "ResourceFileModel.h"

ResourceFileModel::ResourceFileModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

int ResourceFileModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return records.size();
}

QVariant ResourceFileModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }
    if(index.row() >= records.size())
    {
        return QVariant();
    }
    if(role == Qt::DisplayRole)
    {
        const auto name(records.at(index.row()).toObject().value(QStringLiteral("homeworkResourceName")).toString());
        return QVariant::fromValue(name);
    }
    else
    {
        return QVariant();
    }
}

QVariant ResourceFileModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }
    if(orientation == Qt::Horizontal)
    {
        return QStringLiteral("C %0").arg(section);
    }
    else
    {
        return QStringLiteral("R %0").arg(section);
    }
}

#include "AnnouncementModel.h"

AnnouncementModel::AnnouncementModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

int AnnouncementModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return titleList.count();
}

QVariant AnnouncementModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= titleList.count())
    {
        return QVariant();
    }

    switch (role)
    {
    case TitleRole:
        return titleList.at(row);
        break;
    case DetailRole:
        return detailList.at(row);
        break;
    case ReadRole:
        return readList.at(row);
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> AnnouncementModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[DetailRole] = "detail";
    roles[ReadRole] = "read";
    return roles;
}

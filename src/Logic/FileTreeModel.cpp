#include "FileTreeModel.h"
#include "FileTreeItem.h"
#include "../StaticClass/Global.h"

FileTreeModel::FileTreeModel(QObject *parent)
    : QAbstractItemModel{parent},
      rootItem(new FileTreeItem(QFileInfo()))
{
    connect(this, &FileTreeModel::rootPathChanged, this, &FileTreeModel::setupModelData);
    connect(this, &FileTreeModel::sortFlagChanged, this, &FileTreeModel::setupModelData);
}

FileTreeModel::~FileTreeModel()
{
    delete rootItem;
}

QVariant FileTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto item = static_cast<FileTreeItem*>(index.internalPointer());
    if(role == Qt::DisplayRole)
        return item->name;
    else if(role == AbsoluteFilePath)
        return item->fileInfo.absoluteFilePath();
    else
        return QVariant();
}

Qt::ItemFlags FileTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QModelIndex FileTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    FileTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<FileTreeItem*>(parent.internalPointer());

    if (row < 0 || row >= parentItem->childItemsList.size())
        return QModelIndex();
    else
        return createIndex(row, column, parentItem->childItemsList.at(row));
}

QModelIndex FileTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    FileTreeItem *childItem = static_cast<FileTreeItem*>(index.internalPointer());
    FileTreeItem *parentItem = childItem->parentItem;

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int FileTreeModel::rowCount(const QModelIndex &parent) const
{
    FileTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<FileTreeItem*>(parent.internalPointer());

    return parentItem->childItemsList.size();
}

int FileTreeModel::columnCount([[maybe_unused]]const QModelIndex &parent) const
{
    return 1;
}

void FileTreeModel::setupModelData()
{
    beginResetModel();
    for(auto &i : rootItem->childItemsList)
        delete i;
    rootItem->childItemsList.clear();
    rootItem->fileInfo = QFileInfo(rootPath);
    rootItem->name = rootItem->fileInfo.fileName();
    QDir dir(rootPath);
    appendChildItem(rootItem, dir);
    endResetModel();
}

void FileTreeModel::removeFile(const QString &filePath)
{
    if(QFileInfo(filePath).isDir())
        Global::deleteDir(filePath);
    else
        QFile::remove(filePath);
    setupModelData();
}

QHash<int, QByteArray> FileTreeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[AbsoluteFilePath] = "absoluteFilePath";
    roles[Qt::DisplayRole] = "display";
    return roles;
}

void FileTreeModel::appendChildItem(FileTreeItem *parentItem, QDir dir)
{
    const auto fileInfoList(dir.entryInfoList(QDir::Filter::Files | QDir::Filter::Dirs | QDir::Filter::NoDotAndDotDot, static_cast<QDir::SortFlag>(sortFlag)));
    if(fileInfoList.size() < 2)
    {
        for(const auto &i : fileInfoList)
        {
            if(i.isFile())
            {
                parentItem->childItemsList.append(new FileTreeItem(i, parentItem));
            }
            if(i.isDir())
            {
                parentItem->fileInfo = i;
                parentItem->name.append(QStringLiteral("/")).append(i.fileName());
                appendChildItem(parentItem, i.absoluteFilePath());
            }
        }
    }
    else
    {
        for(const auto &i : fileInfoList)
        {
            auto newItem(new FileTreeItem(i, parentItem));
            parentItem->childItemsList.append(newItem);
            if(i.isDir())
            {
                appendChildItem(newItem, i.absoluteFilePath());
            }
        }
    }
}

FileTreeModel::SortFlag FileTreeModel::getSortFlag() const
{
    return sortFlag;
}

void FileTreeModel::setSortFlag(SortFlag newSortFlag)
{
    if (sortFlag == newSortFlag)
        return;
    sortFlag = newSortFlag;
    emit sortFlagChanged();
}

QString FileTreeModel::getRootPath() const
{
    return rootPath;
}

void FileTreeModel::setRootPath(const QString &newRootPath)
{
    if (rootPath == newRootPath)
        return;
    rootPath = newRootPath;
    emit rootPathChanged();
}

#include "FileTreeItem.h"

FileTreeItem::FileTreeItem(const QFileInfo &fileInfo, FileTreeItem *parent)
    : fileInfo(fileInfo),
      parentItem(parent),
      name(fileInfo.fileName())
{

}

FileTreeItem::~FileTreeItem()
{
    for(auto &i : childItemsList)
        delete i;
}

QFileInfo FileTreeItem::getFileInfo() const
{
    return fileInfo;
}

FileTreeItem *FileTreeItem::getParentItem() const
{
    return parentItem;
}

QList<FileTreeItem *> FileTreeItem::getChildItemsList() const
{
    return childItemsList;
}

int FileTreeItem::row() const
{
    if (parentItem)
        return parentItem->childItemsList.indexOf(const_cast<FileTreeItem*>(this));

    return 0;
}

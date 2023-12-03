#ifndef FILETREEITEM_H
#define FILETREEITEM_H


class FileTreeItem
{
public:
    explicit FileTreeItem(const QFileInfo &fileInfo, FileTreeItem *parent = nullptr);
    ~FileTreeItem();
    QFileInfo getFileInfo() const;

    FileTreeItem *getParentItem() const;

    QList<FileTreeItem *> getChildItemsList() const;

    int row() const;

protected:
    friend class FileTreeModel;
    QFileInfo fileInfo;
    FileTreeItem *parentItem = nullptr;
    QList<FileTreeItem*> childItemsList;
    QString name;
};

#endif // FILETREEITEM_H

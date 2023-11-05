#ifndef FILETREEMODEL_H
#define FILETREEMODEL_H

class FileTreeItem;

class FileTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit FileTreeModel(QObject *parent = nullptr);
    ~FileTreeModel();
    enum SortFlag
    {
        Name        = 0x00,
        Time        = 0x01,
        Size        = 0x02,
        Unsorted    = 0x03,
        SortByMask  = 0x03,

        DirsFirst   = 0x04,
        Reversed    = 0x08,
        IgnoreCase  = 0x10,
        DirsLast    = 0x20,
        LocaleAware = 0x40,
        Type        = 0x80,
        NoSort = -1
    };
    enum DataRoles
    {
        AbsoluteFilePath = Qt::UserRole + 1
    };
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QString getRootPath() const;
    void setRootPath(const QString &newRootPath);

    SortFlag getSortFlag() const;
    void setSortFlag(SortFlag newSortFlag);

public slots:
    void setupModelData();
    void removeFile(const QString &filePath);

signals:
    void rootPathChanged();

    void sortFlagChanged();

protected:
    QString rootPath;
    FileTreeItem *rootItem;
    SortFlag sortFlag = SortFlag::Name;

    QHash<int, QByteArray> roleNames() const override;
protected slots:
    void appendChildItem(FileTreeItem *parentItem, QDir dir);
private:
    Q_PROPERTY(QString rootPath READ getRootPath WRITE setRootPath NOTIFY rootPathChanged FINAL)
    Q_PROPERTY(SortFlag sortFlag READ getSortFlag WRITE setSortFlag NOTIFY sortFlagChanged FINAL)
};

#endif // FILETREEMODEL_H

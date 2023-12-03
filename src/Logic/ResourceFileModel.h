#ifndef RESOURCEFILEMODEL_H
#define RESOURCEFILEMODEL_H

class ResourceFileModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ResourceFileModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
protected:
    friend class ResourceFileFetcher;
    QJsonArray records;
};

#endif // RESOURCEFILEMODEL_H

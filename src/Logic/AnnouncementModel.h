#ifndef ANNOUNCEMENTMODEL_H
#define ANNOUNCEMENTMODEL_H

class AnnouncementManager;

class AnnouncementModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit AnnouncementModel(QObject *parent = nullptr);
    enum Dataroles
    {
        TitleRole = Qt::UserRole + 1,
        DetailRole,
        ReadRole
    };
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
protected:
    friend class AnnouncementManager;
    QHash<int, QByteArray> roleNames() const override;
    QStringList titleList;
    QStringList detailList;
    QList<bool> readList;
};

#endif // ANNOUNCEMENTMODEL_H

#ifndef ANNOUNCEMENTMANAGER_H
#define ANNOUNCEMENTMANAGER_H

#include "../Logic/AnnouncementModel.h"

class AnnouncementManager : public QObject
{
    Q_OBJECT
public:
    explicit AnnouncementManager(QObject *parent = nullptr);
    AnnouncementModel *getAnnouncementModel() const;

public slots:
    void obtainAnnouncement();
    void markAnnouncementAsRead(int index);

protected:
    QJsonArray jsonArray;
    AnnouncementModel *announcementModel;

protected slots:
    void analysisRawData(const QByteArray &data);

signals:
    void obtainFinished(int newCount);
    void error(const QString &msg);
private:
    Q_PROPERTY(AnnouncementModel *announcementModel READ getAnnouncementModel CONSTANT FINAL)
};

#endif // ANNOUNCEMENTMANAGER_H

#ifndef LIBRARYUPDATECHECKER_H
#define LIBRARYUPDATECHECKER_H

#include "Version.h"

class LibraryUpdateChecker : public QThread
{
    Q_OBJECT
public:
    explicit LibraryUpdateChecker(QObject *parent = nullptr);
    explicit LibraryUpdateChecker(const QString &currentVersion, QObject *parent = nullptr);

    Version getCurrentVersion() const;

    void setCurrentVersion(const Version &newCurrentVersion);

    Version getNewestVersion() const;

    QString getChangeLog() const;

    bool getRunning() const;

    bool getHasNewVersion() const;

public slots:
    void downloadNewestVersion();

protected:
    Version currentVersion;

    Version newestVersion;
    QString changeLog;

    bool running = false;
    bool hasNewVersion = false;

    void run() override;

signals:
    void checkFinished(bool hasNewVersion);
    void checkFinishedAndHasNewVersion();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished();
};

#endif // LIBRARYUPDATECHECKER_H

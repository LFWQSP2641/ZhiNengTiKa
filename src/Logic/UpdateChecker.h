#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include "Version.h"

class UpdateChecker : public QThread
{
    Q_OBJECT
public:
    explicit UpdateChecker(QObject *parent = nullptr);

    bool checkMinimumVersion() const;

    Version getCurrentVersion() const;

    Version getNewestVersion() const;

    QString getChangeLog() const;

    bool getRunning() const;

    bool getHasNewVersion() const;

public slots:
    void downloadNewestVersion();
    void installNewestVersion();

protected:
    static const Version currentVersion;

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

#endif // UPDATECHECKER_H

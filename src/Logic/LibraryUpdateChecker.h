#ifndef LIBRARYUPDATECHECKER_H
#define LIBRARYUPDATECHECKER_H


class LibraryUpdateChecker : public QThread
{
    Q_OBJECT
public:
    explicit LibraryUpdateChecker(QObject *parent = nullptr);
    explicit LibraryUpdateChecker(const QString &currentVersion, QObject *parent = nullptr);

    QString getCurrentVersion() const;

    void setCurrentVersion(const QString &newCurrentVersion);

    QString getNewestVersion() const;

    QString getChangeLog() const;

    bool getRunning() const;

    bool getHasNewVersion() const;

public slots:
    void downloadNewestVersion();

protected:
    QString currentVersion;

    QString newestVersion;
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

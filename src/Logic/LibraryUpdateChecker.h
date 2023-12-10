#ifndef LIBRARYUPDATECHECKER_H
#define LIBRARYUPDATECHECKER_H


class LibraryUpdateChecker : public QThread
{
    Q_OBJECT
public:
    explicit LibraryUpdateChecker(const QString &currentVersion, QObject *parent = nullptr);

    Q_INVOKABLE QString getCurrentVersion() const;

    Q_INVOKABLE QString getNewestVersion() const;

    Q_INVOKABLE QString getChangeLog() const;

    Q_INVOKABLE bool getRunning() const;

    Q_INVOKABLE bool getHasNewVersion() const;

public slots:
    void downloadNewestVersion();

protected:
    const QString currentVersion;

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

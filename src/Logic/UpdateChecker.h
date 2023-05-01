#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

class UpdateChecker : public QThread
{
    Q_OBJECT
public:
    explicit UpdateChecker(QObject *parent = nullptr);
    // compareVersion, checkMinimumVersion和getCurrentVersion能static
    // 但是QML要调用
    Q_INVOKABLE int compareVersion(const QString &version1, const QString &version2) const;
    Q_INVOKABLE bool checkMinimumVersion() const;

    Q_INVOKABLE QString getCurrentVersion() const;

    Q_INVOKABLE QString getNewestVersion() const;

    Q_INVOKABLE QString getChangeLog() const;

    Q_INVOKABLE bool getRunning() const;

    Q_INVOKABLE bool getHasNewVersion() const;

public slots:
    void downloadNewestVersion();
    void installNewestVersion();

protected:
    static const QString currentVersion;
    QString newestVersion;
    QString changeLog;

    bool running = false;
    bool hasNewVersion = false;

    void run() override;

signals:
    void checkFinished(bool hasNewVersion);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished();
};

#endif // UPDATECHECKER_H

#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

class UpdateChecker : public QThread
{
    Q_OBJECT
public:
    explicit UpdateChecker(QObject *parent = nullptr);
    // version1 < version2, 返回-1
    // version1 = version2, 返回0
    // version1 > version2, 返回1
    static int compareVersion(const QString &version1, const QString &version2);

    bool checkMinimumVersion() const;

    QString getCurrentVersion() const;

    QString getNewestVersion() const;

    QString getChangeLog() const;

    bool getRunning() const;

    bool getHasNewVersion() const;

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
    void checkFinishedAndHasNewVersion();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished();
};

#endif // UPDATECHECKER_H

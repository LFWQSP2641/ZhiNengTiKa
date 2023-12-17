#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include "Version.h"

class UpdateChecker : public QObject
{
    Q_OBJECT
public:
    explicit UpdateChecker(QObject *parent = nullptr);
    explicit UpdateChecker(const Version &currentVersion, QObject *parent = nullptr);

    bool checkMinimumVersion() const;

    Version getCurrentVersion() const;
    void setCurrentVersion(const Version &newCurrentVersion);

    Version getNewestVersion() const;

    QString getChangeLog() const;

    bool isRunning() const;
    bool isFinished() const;

    bool getHasNewVersion() const;

public slots:
    void start();
    void downloadNewestVersion();
    virtual void installNewestVersion() = 0;

protected:
    Version currentVersion;

    Version newestVersion;
    QString changeLog;

    bool running = false;
    bool hasNewVersion = false;

    QBitArray finishedFlag = QBitArray(2, false);

    virtual QString getDomain() const = 0;
    virtual QString getSaveFilePath() const = 0;

protected slots:
    void onNewestVersionReplyFinished();
    void onChangeLogReplyFinished();
    void taskEnding();
    void taskFailing();

    void onFetchDownloadUrlRelpyFinished();
    void onDownloadNewestVersionRelpyFinished();

signals:
    void checkFinished(bool hasNewVersion);
    void checkFinishedAndHasNewVersion();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished();
    void error(const QString &msg);
    void failed();
private:
    Q_PROPERTY(QString changeLog READ getChangeLog CONSTANT FINAL)
    Q_PROPERTY(bool running READ isRunning CONSTANT FINAL)
    Q_PROPERTY(bool hasNewVersion READ getHasNewVersion CONSTANT FINAL)
};

#endif // UPDATECHECKER_H

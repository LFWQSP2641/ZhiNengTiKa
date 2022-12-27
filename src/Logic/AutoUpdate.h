#pragma once


class AutoUpdate : public QObject
{
    Q_OBJECT
public:
    explicit AutoUpdate(QObject *parent = nullptr);
    explicit AutoUpdate(const QString &currentVersion, QObject *parent = nullptr);
    void checkUpdate();
    void setCurrentVersion(const QString &currentVersion)
    {
        this->currentVersion = currentVersion;
    }
    bool isRunning() const
    {
        return running;
    }
    bool isFinished() const
    {
        return !running;
    }

    static bool checkMinimumVersion();

public slots:
    //仅当有新版本时显示
    void showUpdateWidget();
    //所有情况都显示
    void showResultWidget();

private:
    QNetworkReply *newestVersionReply = nullptr;
    QNetworkReply *downloadUrlReply = nullptr;
    QNetworkReply *changeLogReply = nullptr;

    QString newestVersion;
    QString currentVersion;
    QString downloadUrl;
    QString changeLog;

    bool downloadUrlReplyIsFinished = true;
    bool changeLogReplyIsFinished = true;

    static bool compareVersion(const QString &version1, const QString &version2);

    bool running = false;
    bool hasNewVersion = false;

    //int timeoutCount = 0;

private slots:
    void newestVersionReplyFinished();
    void downloadUrlReplyFinished();
    void changeLogReplyFinished();

signals:
    void finished();
};


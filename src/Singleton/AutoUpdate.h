#pragma once


class AutoUpdate : public QObject
{
    Q_OBJECT
public:
    static void initOnce(const QString &currentVersion, QObject *parent = nullptr);
    static AutoUpdate *getInstance();
    bool isRunning() const
    {
        return running;
    }
    bool isFinished() const
    {
        return !running;
    }

    bool checkMinimumVersion();

public slots:
    void checkUpdate(bool showWidget);
    void setCurrentVersion(const QString &currentVersion)
    {
        this->currentVersion = currentVersion;
    }
    //仅当有新版本时显示
    void showUpdateWidget();
    //所有情况都显示
    void showResultWidget();

private:
    static AutoUpdate *instance;
    explicit AutoUpdate(const QString &currentVersion, QObject *parent = nullptr);

    bool showWidgetWhenDoesntHaveUpdate = false;

    QNetworkReply *newestVersionReply = nullptr;
    QNetworkReply *changeLogReply = nullptr;

    QString newestVersion;
    QString currentVersion;
    QString changeLog;

    static bool compareVersion(const QString &version1, const QString &version2);

    bool running = false;
    bool hasNewVersion = false;

    //int timeoutCount = 0;

private slots:
    void newestVersionReplyFinished();
    void changeLogReplyFinished();


signals:
    void finished();
};


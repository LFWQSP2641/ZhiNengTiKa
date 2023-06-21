#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

// (Widgets)提供显示网络图片支持
// (Quick)加快加载速度
class ImageProvider : public QObject
{
    Q_OBJECT
public:
    explicit ImageProvider(QObject *parent = nullptr);
    Q_INVOKABLE QString loadHtml(QString html);
    bool getCacheMode() const;
    void setCacheMode(bool newCacheMode);
    void resetCacheMode();

protected:
    int runningCount = 0;
    bool cacheMode = false;
protected slots:
    void saveFile(QNetworkReply *reply, QString *filePath);
signals:
    void finished();
    void cacheModeChanged();
private:
    Q_PROPERTY(bool cacheMode READ getCacheMode WRITE setCacheMode RESET resetCacheMode NOTIFY cacheModeChanged)
};

#endif // IMAGEPROVIDER_H

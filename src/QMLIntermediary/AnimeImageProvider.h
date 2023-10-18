#ifndef ANIMEIMAGEPROVIDER_H
#define ANIMEIMAGEPROVIDER_H

class AnimeImageProvider : public QQuickImageProvider
{
    Q_OBJECT
public:
    AnimeImageProvider();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

    void fillCache(int index);
    void fillCacheList();
    int getTotalCacheSize() const;
    void setTotalCacheSize(int newTotalCacheSize);

    int getCurrentCacheSize() const;

signals:
    void totalCacheSizeChanged();
    void cacheProgress(int current, int total);
    void firstImageCached();

protected slots:
    void onFillCacheReplyFinished();
    void onttloliPageReplyFinished();
protected:
    int totalCacheSize = 8;
    int currentCacheSize = 0;

    QList<QPair<QImage, bool>> cacheList;
    QHash<QNetworkReply *, int> fillCacheHash;

    static QString replaceRandomNumbers(const QString &input);
private:
    Q_PROPERTY(int totalCacheSize READ getTotalCacheSize WRITE setTotalCacheSize NOTIFY totalCacheSizeChanged FINAL)
    Q_PROPERTY(int currentCacheSize READ getCurrentCacheSize CONSTANT FINAL)
};

#endif // ANIMEIMAGEPROVIDER_H

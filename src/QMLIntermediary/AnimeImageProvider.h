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
protected slots:
    void onFillCacheReplyFinished();
    void onttloliPageReplyFinished();
protected:
    QList<QPair<QImage, bool>> cacheList;
    QHash<QNetworkReply *, int> fillCacheHash;

    static QString replaceRandomNumbers(const QString &input);
};

#endif // ANIMEIMAGEPROVIDER_H

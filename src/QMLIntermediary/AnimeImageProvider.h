#ifndef ANIMEIMAGEPROVIDER_H
#define ANIMEIMAGEPROVIDER_H

class AnimeImageProvider : public QQuickImageProvider
{
public:
    AnimeImageProvider();
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;

    void fillCache(int index);
    void fillCacheList();
protected:
    QReadWriteLock lock;
    QList<QPair<QPixmap, bool>> cacheList;

    static QString replaceRandomNumbers(const QString &input);
};

#endif // ANIMEIMAGEPROVIDER_H

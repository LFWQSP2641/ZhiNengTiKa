#include "AnimeImageProvider.h"
#include "../Singleton/Network.h"
#include "../Singleton/Settings.h"

AnimeImageProvider::AnimeImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap),
      cacheList(32, QPair<QPixmap, bool>(QPixmap(), false))
{
    fillCacheList();
}

QPixmap AnimeImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(id);
    QPixmap pixmap;
    for(auto i(0); i < cacheList.size(); ++i)
    {
        if(cacheList.at(i).second)
        {
            lock.lockForRead();
            pixmap = cacheList.at(i).first;
            lock.unlock();
            lock.lockForWrite();
            cacheList[i].second = false;
            lock.unlock();
            fillCache(i);
            break;
        }
    }
    if(pixmap.isNull())
    {
        pixmap = QPixmap(requestedSize.width() > 0 ? requestedSize.width() : 100,
                         requestedSize.height() > 0 ? requestedSize.height() : 100);
        pixmap.fill(QColor(255, 255, 255, 0).rgba());
    }
    int width = pixmap.width();
    int height = pixmap.height();

    if (size)
        *size = QSize(width, height);
    return pixmap;
}

void AnimeImageProvider::fillCache(int index)
{
    const auto url(replaceRandomNumbers(Settings::getSingletonSettings()->getAnimeImageUrl()));
    auto reply(Network::getGlobalNetworkManager()->getByStrUrl(url));
    QObject::connect(reply, &QNetworkReply::finished, this, [this, index, reply]
    {
        QWriteLocker locker(&lock);
        cacheList[index].first = QPixmap::fromImage(QImage::fromData(Network::getGlobalNetworkManager()->replyReadAll(reply)));
        cacheList[index].second = true;
    }, Qt::DirectConnection);
}

void AnimeImageProvider::fillCacheList()
{
    for(auto i(0); i < cacheList.size(); ++i)
    {
        if(cacheList.at(i).second)
            continue;
        fillCache(i);
    }
}

QString AnimeImageProvider::replaceRandomNumbers(const QString &input)
{
    QString result = input;
    bool ok1, ok2;
    int startPos = 0;
    while ((startPos = result.indexOf("^(", startPos)) != -1)
    {
        int endPos = result.indexOf(")^", startPos + 1);
        if (endPos == -1)
        {
            break; // Invalid pattern, exit the loop
        }

        QString pattern = result.mid(startPos + 1, endPos - startPos);
        QStringList parts = pattern.mid(1, pattern.length() - 2).split("~");
        if (parts.size() == 2)
        {
            int x = parts[0].toInt(&ok1);
            int y = parts[1].toInt(&ok2);

            if (ok1 && ok2)
            {
                if (x <= y)
                {
                    int randomValue = QRandomGenerator::global()->bounded(x, y + 1);
                    result.replace(startPos, endPos - startPos + 2, QString::number(randomValue));
                }
                else
                {
                    int randomValue = QRandomGenerator::global()->bounded(y, x + 1);
                    result.replace(startPos, endPos - startPos + 2, QString::number(randomValue));
                }
            }
            else
            {
                // Invalid numbers, remove the pattern
                result.remove(startPos, endPos - startPos + 2);
            }
        }
        else
        {
            // Invalid pattern, remove the pattern
            result.remove(startPos, endPos - startPos + 2);
        }
        startPos += 1; // Move to the next character after the replaced pattern
    }

    return result;
}

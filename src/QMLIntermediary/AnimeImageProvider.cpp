#include "AnimeImageProvider.h"
#include "../Singleton/Network.h"
#include "../Singleton/Settings.h"

AnimeImageProvider::AnimeImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image),
      cacheList(8, QPair<QImage, bool>(QImage(), false))
{
    fillCacheList();
}

QImage AnimeImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(id);
    QImage image;
    for(auto i(0); i < cacheList.size(); ++i)
    {
        if(cacheList.at(i).second)
        {
            image = cacheList.at(i).first;
            cacheList[i].second = false;
            fillCache(i);
            break;
        }
    }
    if(image.isNull())
    {
        image = QImage(requestedSize.width() > 0 ? requestedSize.width() : 100,
                       requestedSize.height() > 0 ? requestedSize.height() : 100,
                       QImage::Format::Format_ARGB32);
        image.fill(QColor(255, 255, 255, 0).rgba());
    }
    int width = image.width();
    int height = image.height();

    if (size)
        *size = QSize(width, height);
    return image;
}

void AnimeImageProvider::fillCache(int index)
{
    // TODO: 只要QImage在子线程构造, 必报错:
    // Cannot create children for a parent that is in a different thread.
    // (Parent is NetworkAccessManagerBlockable(0x***********), parent's thread is QThread(0x***********), current thread is QQuickPixmapReader(0x***********)
    // 无论是copy还是啥的, 都无法避免
#if 1
    const auto url(replaceRandomNumbers(Settings::getSingletonSettings()->getAnimeImageUrl()));
    auto reply(Network::getGlobalNetworkManager()->getByStrUrl(url));
    fillCacheHash.insert(reply, index);
    connect(reply, &QNetworkReply::finished, this, &AnimeImageProvider::onFillCacheReplyFinished, Qt::QueuedConnection);
#else
    cacheList[index].first = QImage("D:/Downloads/736f354a405b251541368b560cd1f665.jpg");
    cacheList[index].second = true;
#endif
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

void AnimeImageProvider::onFillCacheReplyFinished()
{
    auto reply(qobject_cast<QNetworkReply *>(sender()));
    auto index(fillCacheHash.value(reply));
    if(reply->error() != QNetworkReply::NoError)
    {
        qWarning() << Q_FUNC_INFO << QStringLiteral("reply->error() != QNetworkReply::NoError") << reply->errorString() << reply->readAll();
        reply->deleteLater();
        fillCache(index);
        return;
    }
    cacheList[index].first = QImage::fromData(reply->readAll());
    cacheList[index].second = true;
    reply->deleteLater();
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

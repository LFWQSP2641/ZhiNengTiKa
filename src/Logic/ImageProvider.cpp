#include "ImageProvider.h"
#include "../StaticClass/Global.h"
#include "../Singleton/Network.h"

ImageProvider::ImageProvider(QObject *parent)
    : QObject{parent}
{

}

QString ImageProvider::loadHtml(QString html)
{
    const QStringList imageSuffix({QStringLiteral(".jpg"),
                                   QStringLiteral(".png"),
                                   QStringLiteral(".jpeg")});
    for (auto i{ html.indexOf(QStringLiteral("http")) }; i != -1; i = html.indexOf(QStringLiteral("http"), i + 1))
    {
        //NOTE 因为不知道用的是单引号还是双引号
        //NOTE 并且一定有单引号或双引号
        qsizetype endIndex;
        for(auto j{i + 10};; ++j)
        {
            auto str{html.at(j)};
            if(str == QString(QStringLiteral("'")) || str == QString(QStringLiteral("\"")))
            {
                endIndex = j - 1;
                break;
            }
        }
        const auto imageUrl(html.sliced(i, endIndex - i + 1));
        const auto pointIndex(imageUrl.lastIndexOf("."));
        if(pointIndex == -1)
        {
            continue;
        }
        const auto suffix{imageUrl.last(imageUrl.size() - pointIndex).toLower()};
        if(!imageSuffix.contains(suffix))
        {
            continue;
        }
        const QString imageName(QCryptographicHash::hash(imageUrl.toUtf8(), QCryptographicHash::Sha1).toHex() + suffix);
        auto imagePath(new QString(Global::dataPath().append(QStringLiteral("/Image/")).append(imageName)));
        auto reply(Network::getGlobalNetworkManager()->getByStrUrl(imageUrl));
        ++runningCount;
        connect(reply, &QNetworkReply::finished, [this, imagePath, reply]
        {
            saveFile(reply, imagePath);
        });
        // 不是cacheMode, 则替换
        // 否则, 若文件存在, 则替换
        if((!cacheMode) || QFile(*imagePath).exists())
        {
            html.replace(i, endIndex - i + 1, QStringLiteral("file:///").append(*imagePath));
        }
    }
    return html;
}

bool ImageProvider::getCacheMode() const
{
    return cacheMode;
}

void ImageProvider::setCacheMode(bool newCacheMode)
{
    if (cacheMode == newCacheMode)
        return;
    cacheMode = newCacheMode;
    emit cacheModeChanged();
}

void ImageProvider::resetCacheMode()
{
    setCacheMode(false);
}

void ImageProvider::saveFile(QNetworkReply *reply, QString *filePath)
{
    if(reply->error() != QNetworkReply::NoError)
    {
        --runningCount;
        if(runningCount == 0)
        {
            emit finished();
        }
        return;
    }
    auto hash([](QByteArrayView data)
    {
        return QCryptographicHash::hash(data, QCryptographicHash::Md5);
    });

    const auto data(reply->readAll());
    reply->deleteLater();

    QFile file(*filePath);
    if((!file.open(QFile::ReadOnly)) || (hash(file.readAll()) != hash(data)))
    {
        file.close();
        file.open(QFile::WriteOnly);
        file.write(data);
    }
    file.close();
    delete filePath;
    --runningCount;
    if(runningCount == 0)
    {
        emit finished();
    }
}

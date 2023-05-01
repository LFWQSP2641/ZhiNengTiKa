#include "NetworkAccessManagerBlockable.h"

QByteArray NetworkAccessManagerBlockable::replyReadAll(QNetworkReply *reply)
{
    QByteArray data(reply->readAll());
    reply->deleteLater();
    return data;
}

QNetworkReply *NetworkAccessManagerBlockable::replyWaitForFinished(QNetworkReply *reply)
{
    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    if(reply->isRunning())
    {
        eventLoop.exec();
    }
    return reply;
}

QNetworkReply *NetworkAccessManagerBlockable::getByStrUrl(const QString &strUrl)
{
    return this->getByUrl(strUrl);
}

QNetworkReply *NetworkAccessManagerBlockable::getByUrl(const QUrl &url)
{
    return this->get(QNetworkRequest(url));
}

QNetworkReply *NetworkAccessManagerBlockable::getAndWaitForFinished(const QNetworkRequest &request)
{
    return replyWaitForFinished(this->get(request));
}

QNetworkReply *NetworkAccessManagerBlockable::getAndWaitForFinishedByStrUrl(const QString &strUrl)
{
    return getAndWaitForFinishedByUrl(strUrl);
}

QNetworkReply *NetworkAccessManagerBlockable::getAndWaitForFinishedByUrl(const QUrl &url)
{
    return getAndWaitForFinished(QNetworkRequest(url));
}

QNetworkReply *NetworkAccessManagerBlockable::putAndWaitForFinished(const QNetworkRequest &request, QIODevice *data)
{
    return replyWaitForFinished(this->put(request, data));
}

QNetworkReply *NetworkAccessManagerBlockable::putAndWaitForFinished(const QNetworkRequest &request, const QByteArray &data)
{
    return replyWaitForFinished(this->put(request, data));
}

QNetworkReply *NetworkAccessManagerBlockable::putAndWaitForFinished(const QNetworkRequest &request, QHttpMultiPart *multiPart)
{
    return replyWaitForFinished(this->put(request, multiPart));
}

QNetworkReply *NetworkAccessManagerBlockable::postAndWaitForFinished(const QNetworkRequest &request, QIODevice *data)
{
    return replyWaitForFinished(this->post(request, data));
}

QNetworkReply *NetworkAccessManagerBlockable::postAndWaitForFinished(const QNetworkRequest &request, const QByteArray &data)
{
    return replyWaitForFinished(this->post(request, data));
}

QNetworkReply *NetworkAccessManagerBlockable::postAndWaitForFinished(const QNetworkRequest &request, QHttpMultiPart *multiPart)
{
    return replyWaitForFinished(this->post(request, multiPart));
}

QByteArray NetworkAccessManagerBlockable::getData(const QNetworkRequest &request)
{
    return replyReadAll(getAndWaitForFinished(request));
}

QByteArray NetworkAccessManagerBlockable::getDataByStrUrl(const QString &strUrl)
{
    return getDataByUrl(strUrl);
}

QByteArray NetworkAccessManagerBlockable::getDataByUrl(const QUrl &url)
{
    return getData(QNetworkRequest(url));
}

QByteArray NetworkAccessManagerBlockable::putData(const QNetworkRequest &request, QIODevice *data)
{
    return replyReadAll(putAndWaitForFinished(request, data));
}

QByteArray NetworkAccessManagerBlockable::putData(const QNetworkRequest &request, const QByteArray &data)
{
    return replyReadAll(putAndWaitForFinished(request, data));
}

QByteArray NetworkAccessManagerBlockable::putData(const QNetworkRequest &request, QHttpMultiPart *multiPart)
{
    return replyReadAll(putAndWaitForFinished(request, multiPart));
}

QByteArray NetworkAccessManagerBlockable::postData(const QNetworkRequest &request, QIODevice *data)
{
    return replyReadAll(postAndWaitForFinished(request, data));
}

QByteArray NetworkAccessManagerBlockable::postData(const QNetworkRequest &request, const QByteArray &data)
{
    return replyReadAll(postAndWaitForFinished(request, data));
}

QByteArray NetworkAccessManagerBlockable::postData(const QNetworkRequest &request, QHttpMultiPart *multiPart)
{
    return replyReadAll(postAndWaitForFinished(request, multiPart));
}

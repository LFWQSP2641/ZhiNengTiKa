#include "Network.h"

QNetworkAccessManager Network::networkAccessManager;

QNetworkReply *Network::requestAndWaitForFinished(const QNetworkRequest &requestInfo)
{
    return Network::waitForFinished(Network::networkAccessManager.get(requestInfo));
}

QNetworkReply *Network::postAndWaitForFinished(const QNetworkRequest &requestInfo, const QByteArray &data)
{
    return Network::waitForFinished(Network::networkAccessManager.post(requestInfo, data));
}

QNetworkReply *Network::waitForFinished(QNetworkReply *reply)
{
    QEventLoop eventLoop;
    QObject::connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    if(reply->isRunning())
    {
        eventLoop.exec();
    }
    return reply;
}

QByteArray Network::replyReadAll(QNetworkReply *reply)
{
    auto data{reply->readAll()};
    reply->deleteLater();
    return data;
}

QByteArray Network::getReplyData(QNetworkReply *reply)
{
    waitForFinished(reply);
    return replyReadAll(reply);
}

QByteArray Network::getData(const QNetworkRequest &requestInfo)
{
    return replyReadAll(requestAndWaitForFinished(requestInfo));
}

QByteArray Network::postData(const QNetworkRequest &requestInfo, const QByteArray &data)
{
    return replyReadAll(postAndWaitForFinished(requestInfo, data));
}

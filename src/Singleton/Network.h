#pragma once


class Network
{
public:
    static void initOnce();
    static Network *getInstance();
    QNetworkAccessManager networkAccessManager;
    static QNetworkReply *requestAndWaitForFinished(const QNetworkRequest &requestInfo);
    static QNetworkReply *postAndWaitForFinished(const QNetworkRequest &requestInfo, const QByteArray &data);
    static QNetworkReply *waitForFinished(QNetworkReply *reply);
    static QByteArray replyReadAll(QNetworkReply *reply);
    static QByteArray getReplyData(QNetworkReply *reply);
    static QByteArray getData(const QNetworkRequest &requestInfo);
    static QByteArray postData(const QNetworkRequest &requestInfo, const QByteArray &data);
private:
    Network() = default;
    static Network *instance;
};

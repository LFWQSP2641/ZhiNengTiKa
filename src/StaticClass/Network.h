#pragma once


namespace Network
{
extern QNetworkAccessManager networkAccessManager;
extern QNetworkReply *requestAndWaitForFinished(const QNetworkRequest &requestInfo);
extern QNetworkReply *postAndWaitForFinished(const QNetworkRequest &requestInfo, const QByteArray &data);
extern QNetworkReply *waitForFinished(QNetworkReply *reply);
extern QByteArray replyReadAll(QNetworkReply *reply);
extern QByteArray getReplyData(QNetworkReply *reply);
extern QByteArray getData(const QNetworkRequest &requestInfo);
extern QByteArray postData(const QNetworkRequest &requestInfo, const QByteArray &data);
};


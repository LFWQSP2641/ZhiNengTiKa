#ifndef NETWORKACCESSMANAGERBLOCKABLE_H
#define NETWORKACCESSMANAGERBLOCKABLE_H

class NetworkAccessManagerBlockable : public QNetworkAccessManager
{
    Q_OBJECT
public:
    using QNetworkAccessManager::QNetworkAccessManager;

    QNetworkReply *replyWaitForFinished(QNetworkReply *reply);

    QNetworkReply *getByStrUrl(const QString &strUrl);
    QNetworkReply *getByUrl(const QUrl &url);
    QNetworkReply *getAndWaitForFinished(const QNetworkRequest &request);
    QNetworkReply *getAndWaitForFinishedByStrUrl(const QString &strUrl);
    QNetworkReply *getAndWaitForFinishedByUrl(const QUrl &url);

    QNetworkReply *putAndWaitForFinished(const QNetworkRequest &request, QIODevice *data);
    QNetworkReply *putAndWaitForFinished(const QNetworkRequest &request, const QByteArray &data);
    QNetworkReply *putAndWaitForFinished(const QNetworkRequest &request, QHttpMultiPart *multiPart);

    QNetworkReply *postAndWaitForFinished(const QNetworkRequest &request, QIODevice *data);
    QNetworkReply *postAndWaitForFinished(const QNetworkRequest &request, const QByteArray &data);
    QNetworkReply *postAndWaitForFinished(const QNetworkRequest &request, QHttpMultiPart *multiPart);

    QByteArray replyReadAll(QNetworkReply *reply);
    QByteArray getData(const QNetworkRequest &request);
    QByteArray getDataByStrUrl(const QString &strUrl);
    QByteArray getDataByUrl(const QUrl &url);
    QByteArray putData(const QNetworkRequest &request, QIODevice *data);
    QByteArray putData(const QNetworkRequest &request, const QByteArray &data);
    QByteArray putData(const QNetworkRequest &request, QHttpMultiPart *multiPart);
    QByteArray postData(const QNetworkRequest &request, QIODevice *data);
    QByteArray postData(const QNetworkRequest &request, const QByteArray &data);
    QByteArray postData(const QNetworkRequest &request, QHttpMultiPart *multiPart);

};

#endif // NETWORKACCESSMANAGERBLOCKABLE_H

#pragma once

class UserData;

namespace XinjiaoyuNetwork
{
extern QNetworkRequest setRequest(const QUrl &url, const UserData &userData);
extern QNetworkRequest setRequest(const QUrl &url);
extern QNetworkReply *getTemplateCodeData(const QString &templateCode, const UserData &userData);
extern QNetworkReply *getTemplateCodeData(const QString &templateCode);
extern QByteArray decodeTemplateReply(const QByteArray &rawdata);
extern QString uploadFile(const QByteArray &fileData, const QString &fileName);
extern QNetworkReply* uploadFileReply(const QByteArray &fileData, const QString &fileName);
extern QString getUploadFileReplyUrl(QNetworkReply *reply);
};

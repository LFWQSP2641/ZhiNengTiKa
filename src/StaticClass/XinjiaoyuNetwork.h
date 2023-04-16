#pragma once

class UserData;

namespace XinjiaoyuNetwork
{
extern QNetworkRequest setRequest(const QUrl &url, const UserData &userData);
extern QNetworkRequest setRequest(const QUrl &url);
extern QByteArray getTemplateCodeData(const QString &templateCode, const UserData &userData);
extern QByteArray getTemplateCodeData(const QString &templateCode);
extern QString uploadFile(const QByteArray &fileData, const QString &fileName);
extern QNetworkReply* uploadFileReply(const QByteArray &fileData, const QString &fileName);
extern QString getUploadFileReplyUrl(QNetworkReply *reply);
};

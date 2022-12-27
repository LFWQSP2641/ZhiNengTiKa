#pragma once


namespace XinjiaoyuNetwork
{
extern QNetworkRequest setRequest(const QUrl &url);
extern QNetworkRequest setUserRequest(const QUrl &url);
extern QByteArray getTemplateCodeData(const QString &templateCode, bool useUserAccount = false);
extern QString uploadFile(const QByteArray &fileData, const QString &fileName);
extern QNetworkReply* uploadFileReply(const QByteArray &fileData, const QString &fileName);
};

#pragma once

class UserData
{
public:
    UserData(const QByteArray &accessToken, const QByteArray &authorization, const QByteArray &password, const QByteArray &schoolId, const QJsonObject &sheetData, const QByteArray &studentId, const QByteArray &username)
        : accessTokenStr(accessToken),
          authorizationStr(authorization),
          clientSessionStr(QUuid::createUuid().toString(QUuid::WithoutBraces).toUtf8()),
          passwordStr(password),
          schoolIdStr(schoolId),
          sheetDataJsonObject(sheetData),
          studentIdStr(studentId),
          usernameStr(username) {}
    UserData(const QByteArray &accessToken, const QByteArray &authorization, const QByteArray &clientSession, const QByteArray &password, const QByteArray &schoolId, const QJsonObject &sheetData, const QByteArray &studentId, const QByteArray &username)
        : accessTokenStr(accessToken),
          authorizationStr(authorization),
          clientSessionStr(clientSession),
          passwordStr(password),
          schoolIdStr(schoolId),
          sheetDataJsonObject(sheetData),
          studentIdStr(studentId),
          usernameStr(username) {}

    QByteArray accessToken() const
    {
        return accessTokenStr;
    }
    QByteArray authorization() const
    {
        return authorizationStr;
    }
    QByteArray clientSession() const
    {
        return clientSessionStr;
    }
    QByteArray password() const
    {
        return passwordStr;
    }
    QByteArray schoolId() const
    {
        return schoolIdStr;
    }
    QJsonObject sheetData() const
    {
        return sheetDataJsonObject;
    }
    QByteArray studentId() const
    {
        return studentIdStr;
    }
    QByteArray username() const
    {
        return usernameStr;
    }

private:
    QByteArray accessTokenStr;
    QByteArray authorizationStr;
    QByteArray clientSessionStr;
    QByteArray passwordStr;
    QByteArray schoolIdStr;
    QJsonObject sheetDataJsonObject;
    QByteArray studentIdStr;
    QByteArray usernameStr;
};

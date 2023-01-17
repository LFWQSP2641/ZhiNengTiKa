#pragma once

class UserData
{
public:
    UserData() = default;
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

    void setAccessToken(const QByteArray &newAccessToken)
    {
        accessTokenStr = newAccessToken;
    }
    void setAuthorization(const QByteArray &newAuthorization)
    {
        authorizationStr = newAuthorization;
    }
    void setClientSession(const QByteArray &newClientSession)
    {
        clientSessionStr = newClientSession;
    }
    void setPassword(const QByteArray &newPassword)
    {
        passwordStr = newPassword;
    }
    void setSchoolId(const QByteArray &newSchoolId)
    {
        schoolIdStr = newSchoolId;
    }
    void setSheetData(const QJsonObject &newSheetData)
    {
        sheetDataJsonObject = newSheetData;
    }
    void setStudentId(const QByteArray &newStudentId)
    {
        studentIdStr = newStudentId;
    }
    void setUsername(const QByteArray &newUsername)
    {
        usernameStr = newUsername;
    }

    bool isEmpty() const
    {
        return accessTokenStr.isEmpty() ||
               authorizationStr.isEmpty() ||
               clientSessionStr.isEmpty() ||
               passwordStr.isEmpty() ||
               schoolIdStr.isEmpty() ||
               sheetDataJsonObject.isEmpty() ||
               studentIdStr.isEmpty() ||
               usernameStr.isEmpty();
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

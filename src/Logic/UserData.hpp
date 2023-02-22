#pragma once

class UserData
{
public:
    UserData() = default;
    UserData(const QByteArray &accessToken, const QByteArray &authorization, const QByteArray &clientSession, const QByteArray &password, const QByteArray &schoolId, const QJsonObject &detailData, const QByteArray &studentId, const QByteArray &username)
        : accessTokenStr(accessToken),
          authorizationStr(authorization),
          clientSessionStr(clientSession),
          passwordStr(password),
          schoolIdStr(schoolId),
          detailDataJsonObject(detailData),
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
    QJsonObject detailData() const
    {
        return detailDataJsonObject;
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
    void setDetailData(const QJsonObject &newDetailData)
    {
        detailDataJsonObject = newDetailData;
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
               detailDataJsonObject.isEmpty() ||
               studentIdStr.isEmpty() ||
               usernameStr.isEmpty();
    }

private:
    QByteArray accessTokenStr;
    QByteArray authorizationStr;
    QByteArray clientSessionStr;
    QByteArray passwordStr;
    QByteArray schoolIdStr;
    QJsonObject detailDataJsonObject;
    QByteArray studentIdStr;
    QByteArray usernameStr;
};

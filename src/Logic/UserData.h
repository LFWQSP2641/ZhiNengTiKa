#ifndef USERDATA_H
#define USERDATA_H


class UserData
{
public:
    UserData() = default;
    UserData(const QByteArray &accessToken, const QByteArray &authorization,
             const QByteArray &clientSession, const QByteArray &studentId,
             const QByteArray &password = QByteArray(), const QByteArray &schoolId = QByteArray(),
             const QJsonObject &detailData = QJsonObject(), const QByteArray &username = QByteArray())
        : accessToken(accessToken), authorization(authorization),
          clientSession(clientSession), password(password),
          schoolId(schoolId), detailDataJsonObject(detailData),
          studentId(studentId), username(username) {}

    QByteArray getAccessToken() const
    {
        return accessToken;
    }
    QByteArray getAuthorization() const
    {
        return authorization;
    }
    QByteArray getClientSession() const
    {
        return clientSession;
    }
    QByteArray getPassword() const
    {
        return password;
    }
    QByteArray getSchoolId() const
    {
        return schoolId;
    }
    QJsonObject getDetailDataJsonObject() const
    {
        return detailDataJsonObject;
    }
    QByteArray getStudentId() const
    {
        return studentId;
    }
    QByteArray getUsername() const
    {
        return username;
    }

    bool isEmpty() const
    {
        return accessToken.isEmpty() ||
               authorization.isEmpty() ||
               clientSession.isEmpty();
    }

    static UserData login(const QByteArray &username, const QByteArray &password);
    static void initPublicUserData();
    static UserData getPublicUserData();

private:
    QByteArray accessToken;
    QByteArray authorization;
    QByteArray clientSession;
    QByteArray password;
    QByteArray schoolId;
    QJsonObject detailDataJsonObject;
    QByteArray studentId;
    QByteArray username;

    static QByteArray publicAccessToken;
    static QByteArray publicAuthorization;
    static QByteArray publicClientSession;
    static QByteArray publicStudentId;
};

#endif // USERDATA_H

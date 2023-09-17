#ifndef USERDATA_H
#define USERDATA_H


class UserData
{
public:
    UserData() = default;
    UserData(const QByteArray &accessToken, const QByteArray &authorization,
             const QByteArray &clientSession, const QByteArray &studentId,
             const QJsonObject &detailData = QJsonObject(), const QByteArray &password = QByteArray(),
             const QByteArray &schoolId = QByteArray(), const QByteArray &username = QByteArray())
        : accessToken(accessToken), authorization(authorization),
          clientSession(clientSession), detailDataJsonObject(detailData),
          password(password), schoolId(schoolId),
          studentId(studentId), username(username) {}

    bool relogin();

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
    QString getErrorStr() const;

    bool isEmpty() const
    {
        return accessToken.isEmpty() ||
               authorization.isEmpty() ||
               clientSession.isEmpty();
    }
    bool isValid() const;

    static UserData login(const QByteArray &username, const QByteArray &password);
    static void initPublicUserData();
    static UserData getPublicUserData();


private:
    QByteArray accessToken;
    QByteArray authorization;
    QByteArray clientSession;
    QJsonObject detailDataJsonObject;
    QByteArray password;
    QByteArray schoolId;
    QByteArray studentId;
    QByteArray username;

    QString errorStr;

    static QByteArray publicAccessToken;
    static QByteArray publicAuthorization;
    static QByteArray publicClientSession;
    static QByteArray publicStudentId;
};

#endif // USERDATA_H

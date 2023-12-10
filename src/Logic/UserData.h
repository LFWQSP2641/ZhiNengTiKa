#ifndef USERDATA_H
#define USERDATA_H

class AccountManager;

class UserData
{
    Q_GADGET
public:
    UserData() = default;
    UserData(const QByteArray &accessToken, const QByteArray &authorization,
             const QByteArray &clientSession, const QByteArray &studentId,
             const QJsonObject &detailData = QJsonObject(), const QByteArray &password = QByteArray(),
             const QByteArray &schoolId = QByteArray(), const QByteArray &username = QByteArray())
        : accessToken(accessToken),
          authorization(authorization), clientSession(clientSession),
          detailDataJsonObject(detailData),
          password(password), schoolId(schoolId),
          studentId(studentId), username(username) {}

    bool isEmpty() const
    {
        return accessToken.isEmpty() ||
               authorization.isEmpty() ||
               clientSession.isEmpty();
    }

    Q_INVOKABLE QString getDescription() const;

    QByteArray getAccessToken() const;
    void setAccessToken(const QByteArray &newAccessToken);

    QByteArray getAuthorization() const;
    void setAuthorization(const QByteArray &newAuthorization);

    QByteArray getClientSession() const;
    void setClientSession(const QByteArray &newClientSession);

    QJsonObject getDetailDataJsonObject() const;
    void setDetailDataJsonObject(const QJsonObject &newDetailDataJsonObject);

    QByteArray getPassword() const;
    void setPassword(const QByteArray &newPassword);

    QByteArray getSchoolId() const;
    void setSchoolId(const QByteArray &newSchoolId);

    QByteArray getStudentId() const;
    void setStudentId(const QByteArray &newStudentId);

    QByteArray getUsername() const;
    void setUsername(const QByteArray &newUsername);

protected:
    friend class AccountManager;
    QByteArray accessToken;
    QByteArray authorization;
    QByteArray clientSession;
    QJsonObject detailDataJsonObject;
    QByteArray password;
    QByteArray schoolId;
    QByteArray studentId;
    QByteArray username;

    bool publicUserData = false;

private:
    Q_PROPERTY(QByteArray accessToken READ getAccessToken WRITE setAccessToken FINAL)
    Q_PROPERTY(QByteArray authorization READ getAuthorization WRITE setAuthorization FINAL)
    Q_PROPERTY(QByteArray clientSession READ getClientSession WRITE setClientSession FINAL)
    Q_PROPERTY(QJsonObject detailDataJsonObject READ getDetailDataJsonObject WRITE setDetailDataJsonObject FINAL)
    Q_PROPERTY(QByteArray password READ getPassword WRITE setPassword FINAL)
    Q_PROPERTY(QByteArray schoolId READ getSchoolId WRITE setSchoolId FINAL)
    Q_PROPERTY(QByteArray studentId READ getStudentId WRITE setStudentId FINAL)
    Q_PROPERTY(QByteArray username READ getUsername WRITE setUsername FINAL)
};

Q_DECLARE_METATYPE(UserData)

#endif // USERDATA_H

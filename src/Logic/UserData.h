#ifndef USERDATA_H
#define USERDATA_H

class AccountManager;

class UserData : public QObject
{
    Q_OBJECT
public:
    UserData(QObject *parent = nullptr)
        : QObject{parent} {}
    UserData(const QByteArray &accessToken, const QByteArray &authorization,
             const QByteArray &clientSession, const QByteArray &studentId,
             const QJsonObject &detailData = QJsonObject(), const QByteArray &password = QByteArray(),
             const QByteArray &schoolId = QByteArray(), const QByteArray &username = QByteArray(),
             QObject *parent = nullptr)
        : QObject{parent}, accessToken(accessToken),
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

    QString getDescription() const;

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

signals:
    void accessTokenChanged();

    void authorizationChanged();

    void clientSessionChanged();

    void detailDataJsonObjectChanged();

    void passwordChanged();

    void schoolIdChanged();

    void studentIdChanged();

    void usernameChanged();

private:
    Q_PROPERTY(QByteArray accessToken READ getAccessToken WRITE setAccessToken NOTIFY accessTokenChanged FINAL)
    Q_PROPERTY(QByteArray authorization READ getAuthorization WRITE setAuthorization NOTIFY authorizationChanged FINAL)
    Q_PROPERTY(QByteArray clientSession READ getClientSession WRITE setClientSession NOTIFY clientSessionChanged FINAL)
    Q_PROPERTY(QJsonObject detailDataJsonObject READ getDetailDataJsonObject WRITE setDetailDataJsonObject NOTIFY detailDataJsonObjectChanged FINAL)
    Q_PROPERTY(QByteArray password READ getPassword WRITE setPassword NOTIFY passwordChanged FINAL)
    Q_PROPERTY(QByteArray schoolId READ getSchoolId WRITE setSchoolId NOTIFY schoolIdChanged FINAL)
    Q_PROPERTY(QByteArray studentId READ getStudentId WRITE setStudentId NOTIFY studentIdChanged FINAL)
    Q_PROPERTY(QByteArray username READ getUsername WRITE setUsername NOTIFY usernameChanged FINAL)
};

#endif // USERDATA_H

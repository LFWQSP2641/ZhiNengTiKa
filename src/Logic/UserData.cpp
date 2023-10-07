#include "UserData.h"

QString UserData::getDescription() const
{
    if(publicUserData)
        return QStringLiteral("公共账号");
    else
        return this->getDetailDataJsonObject()
               .value(QStringLiteral("realName"))
               .toString()
               .append(QStringLiteral("  "))
               .append(this->getUsername());
}

QByteArray UserData::getAccessToken() const
{
    return accessToken;
}

void UserData::setAccessToken(const QByteArray &newAccessToken)
{
    if (accessToken == newAccessToken)
        return;
    accessToken = newAccessToken;
    emit accessTokenChanged();
}

QByteArray UserData::getAuthorization() const
{
    return authorization;
}

void UserData::setAuthorization(const QByteArray &newAuthorization)
{
    if (authorization == newAuthorization)
        return;
    authorization = newAuthorization;
    emit authorizationChanged();
}

QByteArray UserData::getClientSession() const
{
    return clientSession;
}

void UserData::setClientSession(const QByteArray &newClientSession)
{
    if (clientSession == newClientSession)
        return;
    clientSession = newClientSession;
    emit clientSessionChanged();
}

QJsonObject UserData::getDetailDataJsonObject() const
{
    return detailDataJsonObject;
}

void UserData::setDetailDataJsonObject(const QJsonObject &newDetailDataJsonObject)
{
    if (detailDataJsonObject == newDetailDataJsonObject)
        return;
    detailDataJsonObject = newDetailDataJsonObject;
    emit detailDataJsonObjectChanged();
}

QByteArray UserData::getPassword() const
{
    return password;
}

void UserData::setPassword(const QByteArray &newPassword)
{
    if (password == newPassword)
        return;
    password = newPassword;
    emit passwordChanged();
}

QByteArray UserData::getSchoolId() const
{
    return schoolId;
}

void UserData::setSchoolId(const QByteArray &newSchoolId)
{
    if (schoolId == newSchoolId)
        return;
    schoolId = newSchoolId;
    emit schoolIdChanged();
}

QByteArray UserData::getStudentId() const
{
    return studentId;
}

void UserData::setStudentId(const QByteArray &newStudentId)
{
    if (studentId == newStudentId)
        return;
    studentId = newStudentId;
    emit studentIdChanged();
}

QByteArray UserData::getUsername() const
{
    return username;
}

void UserData::setUsername(const QByteArray &newUsername)
{
    if (username == newUsername)
        return;
    username = newUsername;
    emit usernameChanged();
}

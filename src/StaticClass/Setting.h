#pragma once


namespace Setting
{
extern void loadFromFile();
extern void saveToFile();

extern QJsonObject settingJsonObject;

extern const QByteArray studentId;
extern QByteArray userStudentId;
extern QByteArray userSchoolId;
extern QByteArray userUsername;
extern QByteArray userPassword;
extern QByteArray userClientSession;
extern QByteArray userAuthorization;
extern QByteArray userAccessToken;
extern const QByteArray clientSession;
extern QByteArray accessToken;
extern QByteArray Authorization;
extern QJsonObject userData;
extern bool listAllTemplate;
extern int fontPointSize;
extern int smallFontPointSize;
extern QString font;
};


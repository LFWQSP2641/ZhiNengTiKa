#pragma once


namespace Setting
{
extern void loadFromFile();
extern void saveToFile();

extern QByteArray userStudentId;
extern QByteArray userSchoolId;
extern QByteArray userUsername;
extern QByteArray userPassword;
extern QByteArray userClientSession;
extern QByteArray userAuthorization;
extern QByteArray userAccessToken;
extern QJsonObject userData;

extern bool listAllTemplate;
extern int fontPointSize;
extern int smallFontPointSize;
extern QString font;
};


#pragma once

#include "../Logic/UserDataList.hpp"

namespace Setting
{
extern void loadFromFile();
extern void saveToFile();

extern bool listAllTemplate;
extern int fontPointSize;
extern int smallFontPointSize;
extern QString font;

extern UserDataList userDataList;
extern QByteArray accessToken();
extern QByteArray authorization();
extern QByteArray clientSession();
extern QByteArray password();
extern QByteArray schoolId();
extern QJsonObject sheetData();
extern QByteArray studentId();
extern QByteArray username();
extern bool logined();
};


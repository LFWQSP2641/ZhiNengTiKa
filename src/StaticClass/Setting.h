#pragma once

#include "../Logic/UserDataList.hpp"

namespace Setting
{
extern void loadFromFile();
extern void saveToFile();

extern bool listLatestTemplatePreferentially;

extern bool compressQRCodeImage;

extern int fontPointSize;
extern int smallFontPointSize;
extern QString font;

extern UserDataList userDataList;
extern bool logined();
extern UserData currentUserData();

extern QJsonObject jsonObjectApiQRCodeScanner;
};


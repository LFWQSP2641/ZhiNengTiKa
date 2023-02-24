#pragma once

#include "../Logic/UserDataList.hpp"

namespace Setting
{
extern void loadFromFile();
extern void saveToFile();

extern bool listAllTemplate;
extern bool getTemplateCodeDataAfterScanQRCodeSuccessfully;
extern bool autoShowDetailWidgetAfterGetTemplateCodeDataSuccessfully;

extern bool compressQRCodeImage;

extern int fontPointSize;
extern int smallFontPointSize;
extern QString font;

extern UserDataList userDataList;
extern bool logined();

extern QJsonObject jsonObjectApiQRCodeScanner;
};


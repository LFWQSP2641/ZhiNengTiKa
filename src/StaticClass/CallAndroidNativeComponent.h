#pragma once

#ifdef Q_OS_ANDROID

namespace CallAndroidNativeComponent
{
QImage takePhoto();
void openUrl(const QString &url);
void installApk(const QString &filePath);
QString getCacheDir();
};

#endif // Q_OS_ANDROID

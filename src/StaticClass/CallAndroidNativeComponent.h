#ifndef CALLANDROIDNATIVECOMPONENT_H
#define CALLANDROIDNATIVECOMPONENT_H

#ifdef Q_OS_ANDROID

namespace CallAndroidNativeComponent
{
QImage takePhoto();
void openUrl(const QString &url);
void installApk(const QString &filePath);
void toSelfSetting();
QString getCacheDir();
QString getAndroidId();
void showToast(const QString &message);
void showToast(const QString &message, int duration);
};

#endif // Q_OS_ANDROID

#endif // CALLANDROIDNATIVECOMPONENT_H

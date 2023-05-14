#ifndef CALLANDROIDNATIVECOMPONENT_H
#define CALLANDROIDNATIVECOMPONENT_H

#ifdef Q_OS_ANDROID

namespace CallAndroidNativeComponent
{
QImage takePhoto();
void openUrl(const QString &url);
void installApk(const QString &filePath);
QString getCacheDir();
QString getAndroidId();
};

#endif // Q_OS_ANDROID

#endif // CALLANDROIDNATIVECOMPONENT_H

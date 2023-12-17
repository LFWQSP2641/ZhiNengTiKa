#include "ApplicationUpdateChecker.h"
#include "../StaticClass/Global.h"
#ifdef Q_OS_ANDROID
#include "../StaticClass/CallAndroidNativeComponent.h"
#endif // Q_OS_ANDROID

ApplicationUpdateChecker::ApplicationUpdateChecker(QObject *parent)
    : UpdateChecker(Version(QStringLiteral(APP_VERSION)), parent),
      domain(QStringLiteral(DATABASE_DOMAIN).append(QStringLiteral("Update/"))),
      saveFilePath(Global::tempPath().append(QStringLiteral("/newVersion.apk")))
{
}

void ApplicationUpdateChecker::installNewestVersion()
{
    qDebug() << Q_FUNC_INFO;
#ifdef Q_OS_ANDROID
    CallAndroidNativeComponent::installApk(saveFilePath);
#endif // Q_OS_ANDROID
}

QString ApplicationUpdateChecker::getDomain() const
{
    return domain;
}

QString ApplicationUpdateChecker::getSaveFilePath() const
{
    return saveFilePath;
}

#include "CallAndroidNativeComponent.h"

#ifdef Q_OS_ANDROID

QImage CallAndroidNativeComponent::takePhoto()
{
    QtAndroidPrivate::requestPermission(QStringLiteral("android.permission.CAMERA")).waitForFinished();
    QJniObject activity = QtAndroidPrivate::activity();
    QJniObject::callStaticMethod<void>(
        "com/LFWQSP2641/ZhiNengTiKa/CallNativeComponent",
        "callNativeCamera",
        "(Lorg/qtproject/qt/android/bindings/QtActivity;)V",
        activity.object<jobject>());
    // 简单粗暴好吧(*/ω＼*)
    QMessageBox::information(Q_NULLPTR, QStringLiteral("information"), QStringLiteral("简单粗暴的线程堵塞,关掉就行"));
    QFile file(CallAndroidNativeComponent::getCacheDir().append(QStringLiteral("/callNativeCamera_result.jpg")));
    if(file.size() < 1)
    {
        qWarning() << "CallAndroidNativeComponent::takePhoto() : file.size() < 1";
        return QImage();
    }
    file.open(QFile::ReadOnly);
    auto image{QImage::fromData(file.readAll())};
    file.close();
    QFileInfo fileInfo(file);
    fileInfo.dir().remove(fileInfo.fileName());
    if(image.isNull())
    {
        qWarning() << "CallAndroidNativeComponent::takePhoto() : image.isNull()";
    }
    return image;
}

void CallAndroidNativeComponent::openUrl(const QString &url)
{
    QJniObject jUrl = QJniObject::fromString(url);
    QJniObject activity = QtAndroidPrivate::activity();

    QJniObject::callStaticMethod<void>(
        "com/LFWQSP2641/ZhiNengTiKa/IntentActivity",
        "openUrl",
        "(Ljava/lang/String;Lorg/qtproject/qt/android/bindings/QtActivity;)V",
        jUrl.object<jstring>(),
        activity.object<jobject>()
    );
}

void CallAndroidNativeComponent::installApk(const QString &filePath)
{
//    QtAndroidPrivate::requestPermission(QStringLiteral("android.permission.WRITE_EXTERNAL_STORAGE")).waitForFinished();
    QtAndroidPrivate::requestPermission(QStringLiteral("android.permission.INSTALL_PACKAGES")).waitForFinished();
    QtAndroidPrivate::requestPermission(QStringLiteral("android.permission.REQUEST_INSTALL_PACKAGES")).waitForFinished();
    QJniObject jFilePath = QJniObject::fromString(filePath);
    QJniObject activity = QtAndroidPrivate::activity();

    QJniObject::callStaticMethod<void>(
        "com/LFWQSP2641/ZhiNengTiKa/IntentActivity",
        "installApk",
        "(Ljava/lang/String;Lorg/qtproject/qt/android/bindings/QtActivity;)V",
        jFilePath.object<jstring>(),
        activity.object<jobject>()
    );
}

void CallAndroidNativeComponent::toSelfSetting()
{
    QJniObject activity = QtAndroidPrivate::activity();

    QJniObject::callStaticMethod<void>(
        "com/LFWQSP2641/ZhiNengTiKa/IntentActivity",
        "toSelfSetting",
        "(Lorg/qtproject/qt/android/bindings/QtActivity;)V",
        activity.object<jobject>()
    );
}

QString CallAndroidNativeComponent::getCacheDir()
{
    QJniObject activity = QtAndroidPrivate::activity();
    return QJniObject::callStaticMethod<jstring>(
               "com/LFWQSP2641/ZhiNengTiKa/Util",
               "getCacheDir",
               "(Lorg/qtproject/qt/android/bindings/QtActivity;)Ljava/lang/String;",
               activity.object<jobject>()).toString();
}

QString CallAndroidNativeComponent::getAndroidId()
{
    QJniObject activity = QtAndroidPrivate::activity();
    return QJniObject::callStaticMethod<jstring>(
               "com/LFWQSP2641/ZhiNengTiKa/Util",
               "getAndroidId",
               "(Lorg/qtproject/qt/android/bindings/QtActivity;)Ljava/lang/String;",
               activity.object<jobject>()).toString();
}

#endif // Q_OS_ANDROID

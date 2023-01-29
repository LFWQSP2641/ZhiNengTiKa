#include "GUI/MainWidget.h"
#include "Logic/AutoUpdate.h"
#include "StaticClass/Global.h"
#include "StaticClass/Setting.h"
#include "StaticClass/CallAndroidNativeComponent.h"
#include "StaticClass/Network.h"
#include "StaticClass/QRCodeScanner.h"

int main(int argc, char *argv[])
{
#ifdef USE_QTWEBVIEW
#ifndef Q_OS_WINDOWS
    QtWebView::initialize();
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGLRhi);
#endif // Q_OS_WINDOWS
#endif // USE_QTWEBVIEW
    QApplication a(argc, argv);

    Network::networkAccessManager = new QNetworkAccessManager(&a);

    Global::initOnce();

    a.setWindowIcon(QIcon(QStringLiteral(":/ico/xinjiaoyuico.png")));

    Setting::loadFromFile();
    QRCodeScanner::initialize(Setting::jsonObjectApiQRCodeScanner);

    QFont appFont;
    if(Setting::fontPointSize < 1 || Setting::font.isEmpty())
    {
        Setting::fontPointSize = a.font().pointSize();
#ifdef Q_OS_ANDROID
        Setting::smallFontPointSize = Setting::fontPointSize / 2;
#else // Q_OS_ANDROID
        Setting::smallFontPointSize = Setting::fontPointSize;
#endif // Q_OS_ANDROID
        Setting::font = a.font().family();
        Setting::saveToFile();
    }
    else
    {
        appFont.setFamily(Setting::font);
        appFont.setPointSize(Setting::fontPointSize);
        a.setFont(appFont);
    }

#ifdef Q_OS_ANDROID
    //删除新版本文件
    QFile file(CallAndroidNativeComponent::getCacheDir() + QDir::separator() + QStringLiteral("newVersion.apk"));
    if(file.exists())
    {
        file.remove();
    }
#endif // Q_OS_ANDROID

    MainWidget w;
    w.show();

#ifdef Q_OS_ANDROID
    w.setEnabled(false);
    AutoUpdate autoUpdate(APP_VERSION);
    QObject::connect(&autoUpdate, &AutoUpdate::finished, &autoUpdate, &AutoUpdate::showUpdateWidget);
    autoUpdate.checkUpdate();
    if(AutoUpdate::checkMinimumVersion())
    {
        w.setEnabled(true);
    }
    else
    {
        QMessageBox::warning(&w, QStringLiteral("warning"), QStringLiteral("请更新版本或检查网络连接"));
        QApplication::exit(1);
    }
#endif // Q_OS_ANDROID

    return a.exec();
}

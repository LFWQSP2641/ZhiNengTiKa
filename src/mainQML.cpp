#include "GUI/MainWidget.h"
#include "Singleton/AutoUpdate.h"
#include "StaticClass/Global.h"
#include "StaticClass/Setting.h"
#include "StaticClass/CallAndroidNativeComponent.h"
#include "Singleton/Network.h"
#include "StaticClass/QRCodeScanner.h"
#include "Logic/MultipleSubjectsTemplateListModelList.h"
#include "Logic/AnalysisWebRawDataQML.h"
#include "Logic/WebRawDataQML.h"

int main(int argc, char *argv[])
{
    QtWebView::initialize();
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGLRhi);
    QApplication a(argc, argv);

    Network::initOnce();

    Global::initOnce();

    a.setWindowIcon(QIcon(QStringLiteral(":/ico/xinjiaoyuico.png")));
    a.setApplicationDisplayName(QStringLiteral("智能题卡"));

    Setting::loadFromFile();
    AutoUpdate::initOnce(APP_VERSION);
    QRCodeScanner::initialize(Setting::jsonObjectApiQRCodeScanner);
    UserData::initPublicUserData();

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
#else
#endif // Q_OS_ANDROID

#ifdef Q_OS_ANDROID
    auto autoUpdate(AutoUpdate::getInstance());
    autoUpdate->checkUpdate(false);
    if(!autoUpdate->checkMinimumVersion())
    {
        QMessageBox::warning(&w, QStringLiteral("warning"), QStringLiteral("请更新版本或检查网络连接"));
        QApplication::exit(1);
        return 1;
    }
#endif

    const auto multipleSubjectsTemplateListModelListInstance {new MultipleSubjectsTemplateListModelList};
    qmlRegisterSingletonInstance("MultipleSubjectsTemplateListModelList", 1, 0, "MultipleSubjectsTemplateListModelList", multipleSubjectsTemplateListModelListInstance);
    qmlRegisterType<AnalysisWebRawDataQML>("AnalysisWebRawDataQML", 1, 0, "AnalysisWebRawData");
    qmlRegisterType<WebRawDataQML>("WebRawDataQML", 1, 0, "WebRawData");
    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/qml/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &a, [url](const QObject * obj, const QUrl & objUrl)
    {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    return a.exec();
}

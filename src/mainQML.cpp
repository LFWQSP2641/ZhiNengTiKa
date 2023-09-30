#include "StaticClass/Global.h"
#include "Singleton/Network.h"
#include "Singleton/Settings.h"
#include "Logic/ImageProvider.h"
#include "Logic/QRCodeScanner.h"
#include "Logic/UpdateChecker.h"
#include "Logic/TemplateSearcher.h"
#include "Logic/TemplateListModel.h"
#include "QMLIntermediary/AnimeImageProvider.h"
#include "QMLIntermediary/MultipleSubjectsTemplateListModelListSingleton.h"
#include "QMLIntermediary/QMLUtils.h"
#include "QMLIntermediary/TemplateSummaryQML.h"
#include "QMLIntermediary/TemplateRawDataQML.h"
#include "QMLIntermediary/TemplateAnalysisQML.h"
#include "QMLIntermediary/SettingOperator.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QtWebView::initialize();
    QApplication a(argc, argv);

    Network::initOnce();

    Global::initOnce();
    // 删除图片缓存
    Global::deleteDir(Global::dataPath().append(QStringLiteral("/Image")));

    a.setWindowIcon(QIcon(QStringLiteral(":/ico/xinjiaoyuico.png")));
    a.setApplicationDisplayName(QStringLiteral("智能题卡"));

    Settings::initOnce();
    auto settings(Settings::getSingletonSettings());
    UserData::initPublicUserData();
    MultipleSubjectsTemplateListModelListSingleton::initOnce();

    QFont appFont;
    if(settings->getFontPointSize() < 1 || settings->getFont().isEmpty())
    {
        settings->setFontPointSize(a.font().pointSize());
        settings->setFont(a.font().family());
        settings->saveToFile();
    }
    else
    {
        appFont.setFamily(settings->getFont());
        appFont.setPointSize(settings->getFontPointSize());
        a.setFont(appFont);
    }

    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE"))
        QQuickStyle::setStyle(settings->getQmlStyle());

    // If this is the first time we're running the application,
    // we need to set a style in the settings so that the QML
    // can find it in the list of built-in styles.
    if (settings->getQmlStyle().isEmpty())
        settings->setQmlStyle(QQuickStyle::name());

#ifdef Q_OS_ANDROID
    //删除新版本文件
    QFile file(Global::tempPath().append(QStringLiteral("/newVersion.apk")));
    if(file.exists())
    {
        file.remove();
    }
#else
#endif // Q_OS_ANDROID

    qmlRegisterSingletonInstance("MultipleSubjectsTemplateListModelList", 1, 0,
                                 "MultipleSubjectsTemplateListModelList",
                                 MultipleSubjectsTemplateListModelListSingleton::getMultipleSubjectsTemplateListModelList());
    qmlRegisterSingletonInstance("Settings", 1, 0, "Settings", settings);
    qmlRegisterSingletonInstance("QMLUtils", 1, 0, "QMLUtils", new QMLUtils);
    qmlRegisterType<TemplateSummaryQML>("TemplateSummaryQML", 1, 0, "TemplateSummaryQML");
    qmlRegisterType<TemplateRawDataQML>("TemplateRawDataQML", 1, 0, "TemplateRawDataQML");
    qmlRegisterType<TemplateAnalysisQML>("TemplateAnalysisQML", 1, 0, "TemplateAnalysisQML");
    qmlRegisterType<TemplateSearcher>("TemplateSearcher", 1, 0, "TemplateSearcher");
    qmlRegisterType<TemplateListModel>("TemplateListModel", 1, 0, "TemplateListModel");
    qmlRegisterType<SettingOperator>("SettingOperator", 1, 0, "SettingOperator");
    qmlRegisterType<UpdateChecker>("UpdateChecker", 1, 0, "UpdateChecker");
    qmlRegisterType<QRCodeScanner>("QRCodeScanner", 1, 0, "QRCodeScanner");
    qmlRegisterType<ImageProvider>("ImageProvider", 1, 0, "ImageProvider");
    QQmlApplicationEngine engine;
    engine.addImageProvider(QStringLiteral("AnimeImageProvider"), new AnimeImageProvider);

    QStringList builtInStyles = { QStringLiteral("Basic"), QStringLiteral("Fusion"),
                                  QStringLiteral("Imagine"), QStringLiteral("Material"), QStringLiteral("Universal")
                                };
#if defined(Q_OS_MACOS)
    builtInStyles << QStringLiteral("macOS");
    builtInStyles << QStringLiteral("iOS");
#elif defined(Q_OS_IOS)
    builtInStyles << QStringLiteral("iOS");
#elif defined(Q_OS_WINDOWS)
    builtInStyles << QStringLiteral("Windows");
#endif

    engine.setInitialProperties({{ QStringLiteral("builtInStyles"), builtInStyles }});

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &a, [url](const QObject * obj, const QUrl & objUrl)
    {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    return a.exec();
}

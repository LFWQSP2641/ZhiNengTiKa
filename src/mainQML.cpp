#include "StaticClass/Global.h"
#include "StaticClass/CallAndroidNativeComponent.h"
#include "Singleton/Network.h"
#include "Singleton/Settings.h"
#include "Logic/AccountManager.h"
#include "Logic/AnnouncementManager.h"
#include "Logic/FileTreeModel.h"
#include "Logic/ImageProvider.h"
#include "Logic/QRCodeScanner.h"
#include "Logic/ResourceFileFetcher.h"
#include "Logic/Version.h"
#include "Logic/TemplateSummary.h"
#include "Logic/TemplateAnalysis.h"
#include "Logic/TemplateFetcher.h"
#include "Logic/TemplateSearcher.h"
#include "Logic/TemplateListModel.h"
#include "Logic/UserData.h"
#include "QMLIntermediary/AccelerometerSingleton.h"
#include "QMLIntermediary/AnimeImageProvider.h"
#include "QMLIntermediary/MultipleSubjectsTemplateListModelListSingleton.h"
#include "QMLIntermediary/QMLUtils.h"
#include "QMLIntermediary/ZAccelerationToOpacityConverter.h"
#include "QMLIntermediary/SettingOperator.h"
#include "LibZXingCpp/ZXingResult.h"

// TODO 记得删除
bool newVersionLauncher = false;

extern "C" Q_DECL_EXPORT const char *getVersion()
{
    return APP_VERSION;
}

extern "C" Q_DECL_EXPORT void setLauncherVersion(const char *version)
{
    const QString launcherVersionStr(version);
    Version launcherVersion(launcherVersionStr);
    if(launcherVersionStr.isEmpty() || launcherVersion < Version(QStringLiteral("3.0.2")))
        newVersionLauncher = false;
    else
        newVersionLauncher = true;
    qDebug() << newVersionLauncher;
}

extern "C" Q_DECL_EXPORT int run(QApplication *a)
{
    QEventLoop libEventLoop;

    AccelerometerSingleton::initOnce(&libEventLoop);

    Network::initOnce();

    Global::initOnce();
    // 删除图片缓存
    Global::deleteDir(Global::dataPath().append(QStringLiteral("/Image")));

    a->setWindowIcon(QIcon(QStringLiteral(":/ico/xinjiaoyuico.png")));
    a->setApplicationDisplayName(QStringLiteral("智能题卡"));

    Settings::initOnce();
    auto settings(Settings::getSingletonSettings());
    MultipleSubjectsTemplateListModelListSingleton::initOnce();

    QFont appFont;
    if(settings->getFontPointSize() < 1 || settings->getFont().isEmpty())
    {
        settings->setFontPointSize(a->font().pointSize());
        settings->setFont(a->font().family());
        settings->saveToFile();
    }
    else
    {
        appFont.setFamily(settings->getFont());
        appFont.setPointSize(settings->getFontPointSize());
        a->setFont(appFont);
    }

    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE"))
        QQuickStyle::setStyle(settings->getQmlStyle());

    // If this is the first time we're running the application,
    // we need to set a style in the settings so that the QML
    // can find it in the list of built-in styles.
    if (settings->getQmlStyle().isEmpty())
        settings->setQmlStyle(QQuickStyle::name());

    if(!newVersionLauncher)
    {
        QMessageBox msgb;
        msgb.setText(QStringLiteral("安装新版本中...\n稍安勿躁"));
        msgb.show();
        const QString saveFilePath(Global::tempPath().append(QStringLiteral("/newVersion.apk")));
        auto newestVersion(Network::getGlobalNetworkManager()->getDataByStrUrl(Network::getGlobalNetworkManager()->getDataByStrUrl(QStringLiteral("getNewestVersionEncryption").prepend(QStringLiteral(DATABASE_DOMAIN)))));

        newestVersion[4] = static_cast<char>(120);
        newestVersion[5] = static_cast<char>(156);
        QFile file(saveFilePath);
        file.open(QFile::WriteOnly);
        file.write(qUncompress(newestVersion));
        file.close();

#ifdef Q_OS_ANDROID
        CallAndroidNativeComponent::installApk(saveFilePath);
#endif // Q_OS_ANDROID
    }

    auto animeImageProvider(new AnimeImageProvider);
    auto qmlUtils(new QMLUtils(&libEventLoop));

    qmlRegisterSingletonInstance("MultipleSubjectsTemplateListModelList", 1, 0,
                                 "MultipleSubjectsTemplateListModelList",
                                 MultipleSubjectsTemplateListModelListSingleton::getMultipleSubjectsTemplateListModelList());
    qmlRegisterSingletonInstance("Settings", 1, 0, "Settings", settings);
    qmlRegisterSingletonInstance("AccountManager", 1, 0, "AccountManager", settings->getAccountManager());
    qmlRegisterSingletonInstance("QMLUtils", 1, 0, "QMLUtils", qmlUtils);
    qmlRegisterSingletonInstance("AnimeImageProvider", 1, 0, "AnimeImageProvider", animeImageProvider);
    qmlRegisterSingletonInstance("LibEventLoop", 1, 0, "LibEventLoop", &libEventLoop);
    qRegisterMetaType<TemplateSummary>("TemplateSummary");
    qRegisterMetaType<TemplateAnalysis>("TemplateAnalysis");
    qRegisterMetaType<UserData>("UserData");
    qRegisterMetaType<ZXingResult>("ZXingResult");
    qmlRegisterType<TemplateFetcher>("TemplateFetcher", 1, 0, "TemplateFetcher");
    qmlRegisterType<TemplateSearcher>("TemplateSearcher", 1, 0, "TemplateSearcher");
    qmlRegisterType<TemplateListModel>("TemplateListModel", 1, 0, "TemplateListModel");
    qmlRegisterType<SettingOperator>("SettingOperator", 1, 0, "SettingOperator");
    qmlRegisterType<QRCodeScanner>("QRCodeScanner", 1, 0, "QRCodeScanner");
    qmlRegisterType<ImageProvider>("ImageProvider", 1, 0, "ImageProvider");
    qmlRegisterType<ZAccelerationToOpacityConverter>("ZAccelerationToOpacityConverter", 1, 0, "ZAccelerationToOpacityConverter");
    qmlRegisterType<AnnouncementManager>("AnnouncementManager", 1, 0, "AnnouncementManager");
    qmlRegisterType<ResourceFileFetcher>("ResourceFileFetcher", 1, 0, "ResourceFileFetcher");
    qmlRegisterType<FileTreeModel>("FileTreeModel", 1, 0, "FileTreeModel");
    QQmlApplicationEngine engine;
    engine.addImageProvider(QStringLiteral("AnimeImageProvider"), animeImageProvider);

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
                     &libEventLoop, [url, &libEventLoop](const QObject * obj, const QUrl & objUrl)
    {
        if (!obj && url == objUrl)
            libEventLoop.exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return libEventLoop.exec();
}

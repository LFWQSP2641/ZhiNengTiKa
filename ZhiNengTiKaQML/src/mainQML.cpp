#include "QMLIntermediary/AnimeImageProvider.h"
#include "QMLIntermediary/QMLUtils.h"
#include "QMLIntermediary/SettingOperator.h"
#include "src/Logic/AnnouncementManager.h"
#include "src/Logic/ImageProviderMultiThreading.h"
#include "src/Logic/QRCodeScanner.h"
#include "src/Logic/ResourceFileFetcher.h"
#include "src/Logic/TemplateHandle/ListModel/TemplateListModel.h"
#include "src/Logic/TemplateHandle/TemplateAnalysis.h"
#include "src/Logic/TemplateHandle/TemplateFetcher.h"
#include "src/Logic/TemplateHandle/TemplateModel/TemplateDataTreeModel.h"
#include "src/Logic/TemplateHandle/TemplateSummary.h"
#include "src/Logic/TemplateSearcher.h"
#include "src/Logic/UserData.h"
#include "src/Singleton/MultipleSubjectsTemplateListModelListSingleton.h"
#include "src/Singleton/Network.h"
#include "src/Singleton/Settings.h"
#include "src/StaticClass/Global.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Global::initOnce();

    a.setWindowIcon(QIcon(QStringLiteral(":/ico/xinjiaoyuico.png")));
    a.setApplicationDisplayName(QStringLiteral("智能题卡"));

    Network::initOnce();
    auto settings(Settings::getSingletonSettings());

    QFont appFont;
    if (settings->getFontPointSize() < 1 || settings->getFont().isEmpty())
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

    auto animeImageProvider(new AnimeImageProvider);
    auto qmlUtils(new QMLUtils(&a));

    qmlRegisterSingletonInstance("MultipleSubjectsTemplateListModelList", 1, 0,
                                 "MultipleSubjectsTemplateListModelList",
                                 MultipleSubjectsTemplateListModelListSingleton::getMultipleSubjectsTemplateListModelList());
    qmlRegisterSingletonInstance("Settings", 1, 0, "Settings", settings);
    qmlRegisterSingletonInstance("AccountManager", 1, 0, "AccountManager", settings->getAccountManager());
    qmlRegisterSingletonInstance("QMLUtils", 1, 0, "QMLUtils", qmlUtils);
    qmlRegisterSingletonInstance("AnimeImageProvider", 1, 0, "AnimeImageProvider", animeImageProvider);
    qRegisterMetaType<TemplateSummary>("TemplateSummary");
    qRegisterMetaType<TemplateAnalysis>("TemplateAnalysis");
    qRegisterMetaType<UserData>("UserData");
    qRegisterMetaType<ZXingResult>("ZXingResult");
    qmlRegisterType<TemplateFetcher>("TemplateFetcher", 1, 0, "TemplateFetcher");
    qmlRegisterType<TemplateSearcher>("TemplateSearcher", 1, 0, "TemplateSearcher");
    qmlRegisterType<TemplateListModel>("TemplateListModel", 1, 0, "TemplateListModel");
    qmlRegisterType<TemplateDataTreeModel>("TemplateDataTreeModel", 1, 0, "TemplateDataTreeModel");
    qmlRegisterType<SettingOperator>("SettingOperator", 1, 0, "SettingOperator");
    qmlRegisterType<QRCodeScanner>("QRCodeScanner", 1, 0, "QRCodeScanner");
    qmlRegisterType<ImageProviderMultiThreading>("ImageProviderMultiThreading", 1, 0, "ImageProviderMultiThreading");
    qmlRegisterType<AnnouncementManager>("AnnouncementManager", 1, 0, "AnnouncementManager");
    qmlRegisterType<ResourceFileFetcher>("ResourceFileFetcher", 1, 0, "ResourceFileFetcher");
    QQmlApplicationEngine engine;
    engine.addImageProvider(QStringLiteral("AnimeImageProvider"), animeImageProvider);

    QStringList builtInStyles = { QStringLiteral("Basic"), QStringLiteral("Fusion"),
                                  QStringLiteral("Imagine"), QStringLiteral("Material"), QStringLiteral("Universal") };
#if defined(Q_OS_MACOS)
    builtInStyles << QStringLiteral("macOS");
    builtInStyles << QStringLiteral("iOS");
#elif defined(Q_OS_IOS)
    builtInStyles << QStringLiteral("iOS");
#elif defined(Q_OS_WINDOWS)
    builtInStyles << QStringLiteral("Windows");
#endif

    engine.setInitialProperties({
        {QStringLiteral("builtInStyles"), builtInStyles}
    });

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated,
        &a, [url, &a](const QObject *obj, const QUrl &objUrl)
        {
        if (!obj && url == objUrl)
            a.exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return a.exec();
}

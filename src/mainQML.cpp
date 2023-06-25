#include "StaticClass/Global.h"
#include "Singleton/Network.h"
#include "Singleton/Settings.h"
#include "Logic/ImageProvider.h"
#include "Logic/UpdateChecker.h"
#include "Logic/TemplateSearcher.h"
#include "Logic/TemplateListModel.h"
#include "QMLIntermediary/MultipleSubjectsTemplateListModelListSingleton.h"
#include "QMLIntermediary/QRCodeScannerQML.h"
#include "QMLIntermediary/TemplateSummaryQML.h"
#include "QMLIntermediary/TemplateRawDataQML.h"
#include "QMLIntermediary/TemplateAnalysisQML.h"
#include "QMLIntermediary/SettingOperator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Network::initOnce();

    Global::initOnce();

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
    qmlRegisterType<TemplateSummaryQML>("TemplateSummaryQML", 1, 0, "TemplateSummaryQML");
    qmlRegisterType<TemplateRawDataQML>("TemplateRawDataQML", 1, 0, "TemplateRawDataQML");
    qmlRegisterType<TemplateAnalysisQML>("TemplateAnalysisQML", 1, 0, "TemplateAnalysisQML");
    qmlRegisterType<TemplateSearcher>("TemplateSearcher", 1, 0, "TemplateSearcher");
    qmlRegisterType<TemplateListModel>("TemplateListModel", 1, 0, "TemplateListModel");
    qmlRegisterType<SettingOperator>("SettingOperator", 1, 0, "SettingOperator");
    qmlRegisterType<UpdateChecker>("UpdateChecker", 1, 0, "UpdateChecker");
    qmlRegisterType<QRCodeScannerQML>("QRCodeScannerQML", 1, 0, "QRCodeScannerQML");
    qmlRegisterType<ImageProvider>("ImageProvider", 1, 0, "ImageProvider");
    QQmlApplicationEngine engine;

    QStringList builtInStyles = { QLatin1String("Basic"), QLatin1String("Fusion"),
                                  QLatin1String("Imagine"), QLatin1String("Material"), QLatin1String("Universal")
                                };
#if defined(Q_OS_MACOS)
    builtInStyles << QLatin1String("macOS");
    builtInStyles << QLatin1String("iOS");
#elif defined(Q_OS_IOS)
    builtInStyles << QLatin1String("iOS");
#elif defined(Q_OS_WINDOWS)
    builtInStyles << QLatin1String("Windows");
#endif

    engine.setInitialProperties({{ "builtInStyles", builtInStyles }});

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

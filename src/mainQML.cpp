#include "StaticClass/Global.h"
#include "StaticClass/Setting.h"
#include "Singleton/Network.h"
#include "StaticClass/QRCodeScanner.h"
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
    QtWebView::initialize();
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGLRhi);
    QApplication a(argc, argv);

    Network::initOnce();

    Global::initOnce();

    a.setWindowIcon(QIcon(QStringLiteral(":/ico/xinjiaoyuico.png")));
    a.setApplicationDisplayName(QStringLiteral("智能题卡"));

    Setting::loadFromFile();
    QRCodeScanner::initialize(Setting::jsonObjectApiQRCodeScanner);
    UserData::initPublicUserData();
    MultipleSubjectsTemplateListModelListSingleton::initOnce();

    QFont appFont;
    if(Setting::fontPointSize < 1 || Setting::font.isEmpty())
    {
        Setting::fontPointSize = a.font().pointSize();
        Setting::font = a.font().family();
        Setting::saveToFile();
    }
    else
    {
        appFont.setFamily(Setting::font);
        appFont.setPointSize(Setting::fontPointSize);
        a.setFont(appFont);
    }

    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE"))
        QQuickStyle::setStyle(Setting::qmlStyle);

    // If this is the first time we're running the application,
    // we need to set a style in the settings so that the QML
    // can find it in the list of built-in styles.
    if (Setting::qmlStyle.isEmpty())
        Setting::qmlStyle = QQuickStyle::name();

#ifdef Q_OS_ANDROID
    //删除新版本文件
    QFile file(Global::tempPath() + QStringLiteral("/") + QStringLiteral("newVersion.apk"));
    if(file.exists())
    {
        file.remove();
    }
#else
#endif // Q_OS_ANDROID

    qmlRegisterSingletonInstance("MultipleSubjectsTemplateListModelList", 1, 0, "MultipleSubjectsTemplateListModelList", MultipleSubjectsTemplateListModelListSingleton::getMultipleSubjectsTemplateListModelList());
    qmlRegisterSingletonInstance("QRCodeScannerQML", 1, 0, "QRCodeScanner", new QRCodeScannerQML);
    qmlRegisterType<TemplateSummaryQML>("TemplateSummaryQML", 1, 0, "TemplateSummaryQML");
    qmlRegisterType<TemplateRawDataQML>("TemplateRawDataQML", 1, 0, "TemplateRawDataQML");
    qmlRegisterType<TemplateAnalysisQML>("TemplateAnalysisQML", 1, 0, "TemplateAnalysisQML");
    qmlRegisterType<TemplateSearcher>("TemplateSearcher", 1, 0, "TemplateSearcher");
    qmlRegisterType<TemplateListModel>("TemplateListModel", 1, 0, "TemplateListModel");
    qmlRegisterType<SettingOperator>("SettingOperator", 1, 0, "SettingOperator");
    qmlRegisterType<UpdateChecker>("UpdateChecker", 1, 0, "UpdateChecker");
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

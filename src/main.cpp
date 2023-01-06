#include "GUI/MainWidget.h"
#include "Logic/AutoUpdate.h"
#include "StaticClass/Global.h"
#include "StaticClass/Setting.h"

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

#ifdef Q_OS_ANDROID
    Global::appConfigPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    Global::appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    Global::appTempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
#else
    Global::appConfigPath = QApplication::applicationDirPath() + QDir::separator() + QStringLiteral("Config");
    Global::appDataPath = QApplication::applicationDirPath() + QDir::separator() + QStringLiteral("Data");
    Global::appTempPath = QApplication::applicationDirPath() + QDir::separator() + QStringLiteral("Temp");
#endif // Q_OS_ANDROID

    QDir dir;
    dir.mkdir(Global::configPath());
    dir.mkdir(Global::dataPath());
    dir.mkdir(Global::tempPath());
    dir.mkdir(Global::tempPath() + QDir::separator() + QStringLiteral("Image"));
    dir.mkdir(Global::tempPath() + QDir::separator() + QStringLiteral("TemplateFile"));

    a.setWindowIcon(QIcon(QStringLiteral(":/ico/xinjiaoyuico.png")));

    Setting::loadFromFile();

    QFont appFont;
    if(Setting::fontPointSize < 1 || Setting::font.isEmpty())
    {
        Setting::fontPointSize = a.font().pointSize();
#ifdef Q_OS_ANDROID
        Setting::smallFontPointSize = Setting::fontPointSize / 2;
#else
        Setting::smallFontPointSize = Setting::fontPointSize;
#endif
        Setting::font = a.font().family();
        Setting::saveToFile();
    }
    else
    {
        appFont.setFamily(Setting::font);
        appFont.setPointSize(Setting::fontPointSize);
        a.setFont(appFont);
    }
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

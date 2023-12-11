#include "Logic/LibraryUpdateChecker.h"
#include "Logic/UpdateChecker.h"
#include "StaticClass/Global.h"
#include "Singleton/Network.h"
#ifdef Q_OS_ANDROID
#include "StaticClass/CallAndroidNativeComponent.h"
#endif

void showMessage(const QString &message)
{
#ifdef Q_OS_ANDROID
    CallAndroidNativeComponent::showToast(message);
#else // Q_OS_ANDROID
    QMessageBox::information(nullptr, {}, message);
#endif // Q_OS_ANDROID
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QtWebView::initialize();
    QApplication a(argc, argv);

    Global::initOnce();
    Network::initOnce();

    UpdateChecker updateChecker(&a);
    if(!updateChecker.checkMinimumVersion())
    {
        showMessage(QStringLiteral("请检查网络状态或更新"));
        return 1;
    }
    LibraryUpdateChecker libraryUpdateChecker(&a);

    QObject::connect(&updateChecker, &UpdateChecker::checkFinished, &a, [&updateChecker, &libraryUpdateChecker](bool hasNewVersion)
    {
        if(hasNewVersion)
        {
            showMessage(QStringLiteral("启动器有新版本, 下载中..."));
            updateChecker.downloadNewestVersion();
            return;
        }
        if(libraryUpdateChecker.isFinished() && libraryUpdateChecker.getHasNewVersion())
        {
            libraryUpdateChecker.downloadNewestVersion();
            return;
        }
    });
    QObject::connect(&updateChecker, &UpdateChecker::downloadFinished, &updateChecker, &UpdateChecker::installNewestVersion);

    QObject::connect(&libraryUpdateChecker, &LibraryUpdateChecker::checkFinishedAndHasNewVersion, &a, [&updateChecker, &libraryUpdateChecker]
    {
        if((!updateChecker.isFinished()) || updateChecker.getHasNewVersion())
            return;
        if(libraryUpdateChecker.getHasNewVersion())
        {
            libraryUpdateChecker.downloadNewestVersion();
            return;
        }
    });
    updateChecker.start();

    const QString libraryFilePath(Global::dataPath().append(QStringLiteral("/libZhiNengTiKaQML_arm64-v8a.so")));
    const QString libraryPath(Global::dataPath().append(QStringLiteral("/libZhiNengTiKaQML_arm64-v8a")));

    QFile libraryFile(libraryFilePath);
    if(!libraryFile.exists())
    {
        QEventLoop eventLoop;
        QObject::connect(&libraryUpdateChecker, &LibraryUpdateChecker::downloadFinished, &eventLoop, [&eventLoop]
        {
            showMessage(QStringLiteral("下载主程序动态库文件成功"));
            eventLoop.quit();
        }, Qt::SingleShotConnection);
        libraryUpdateChecker.start();
        eventLoop.exec();
    }
    QFile newVersionLibraryFile(Global::dataPath().append(QStringLiteral("/newVersionLibrary")));
    if(newVersionLibraryFile.exists())
    {
        libraryFile.remove();
        newVersionLibraryFile.copy(libraryFilePath);
        newVersionLibraryFile.remove();
    }

    QObject::connect(&libraryUpdateChecker, &LibraryUpdateChecker::downloadFinished, &a, []
    {
        showMessage(QStringLiteral("下载主程序动态库文件成功, 将在下次启动时自动更新"));
    });

    QLibrary library(libraryPath);
    if(!library.load())
    {
        showMessage(QStringLiteral("load library failed\n")
                    .append(library.errorString()));
    }
    else
    {
        typedef void (*SetLauncherVersion)(const char *);
        SetLauncherVersion setLauncherVersion = (SetLauncherVersion)library.resolve("setLauncherVersion");

        if(setLauncherVersion)
        {
            setLauncherVersion(APP_VERSION);
        }

        typedef const char * (*GetZhiNengTiKaQMLVersion)();
        GetZhiNengTiKaQMLVersion getZhiNengTiKaQMLVersion = (GetZhiNengTiKaQMLVersion)library.resolve("getVersion");

        if(getZhiNengTiKaQMLVersion)
        {
            const QString currentVersion(getZhiNengTiKaQMLVersion());
            qDebug() << "Library Version:" << currentVersion;
            libraryUpdateChecker.setCurrentVersion(Version(currentVersion));
            libraryUpdateChecker.start();
        }

        typedef int (*ZhiNengTiKaQML)(QApplication *);
        ZhiNengTiKaQML zhiNengTiKaQML = (ZhiNengTiKaQML)library.resolve("run");

        if(zhiNengTiKaQML)
        {
            auto result(zhiNengTiKaQML(&a));
            showMessage(QStringLiteral("主程序函数退出, 本程序即将关闭"));
            if(!libraryUpdateChecker.isFinished())
            {
                libraryUpdateChecker.quit();
                libraryUpdateChecker.wait();
            }
            QTimer::singleShot(3000, &a, [result]
            {
                QApplication::exit(result);
            });
        }
    }
    return a.exec();
}

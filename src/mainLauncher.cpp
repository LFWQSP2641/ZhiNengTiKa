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

    QObject::connect(&updateChecker, &UpdateChecker::checkFinished, &a, [&updateChecker, &a, &libraryUpdateChecker](bool hasNewVersion)
    {
        if(hasNewVersion)
        {
            updateChecker.downloadNewestVersion();
            showMessage(QStringLiteral("启动器有新版本, 下载中..."));
            a.exec();
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
        typedef const char * (*GetZhiNengTiKaQMLVersion)();
        GetZhiNengTiKaQMLVersion getZhiNengTiKaQMLVersion = (GetZhiNengTiKaQMLVersion)library.resolve("getVersion");

        if(getZhiNengTiKaQMLVersion)
        {
            const QString currentVersion(getZhiNengTiKaQMLVersion());
            qDebug() << "Library Version:" << currentVersion;
            libraryUpdateChecker.setCurrentVersion(currentVersion);
            libraryUpdateChecker.start();
        }

        typedef int (*ZhiNengTiKaQML)(QApplication *);
        ZhiNengTiKaQML zhiNengTiKaQML = (ZhiNengTiKaQML)library.resolve("run");

        if(zhiNengTiKaQML)
        {
            auto result(zhiNengTiKaQML(&a));
            if(!libraryUpdateChecker.isFinished())
            {
                libraryUpdateChecker.quit();
                libraryUpdateChecker.wait();
            }
            return result;
        }
    }
    return -1;
}

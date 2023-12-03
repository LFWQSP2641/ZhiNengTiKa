#include <QMessageBox>
#include "Logic/LibraryUpdateChecker.h"
#include "Logic/UpdateChecker.h"
#include "StaticClass/Global.h"
#include "Singleton/Network.h"

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
        QMessageBox::critical(nullptr, {}, QStringLiteral("请检查网络状态或更新"));
        return 1;
    }
    QObject::connect(&updateChecker, &UpdateChecker::checkFinishedAndHasNewVersion, [&updateChecker]()
    {
        QMessageBox::information(nullptr, {}, QStringLiteral("启动器有新版本, 请更新\n\n")
                                 .append(updateChecker.getChangeLog()));
        updateChecker.downloadNewestVersion();
    });
    QObject::connect(&updateChecker, &UpdateChecker::downloadFinished, &updateChecker, &UpdateChecker::installNewestVersion);
    updateChecker.start();

    QFile libraryFile(Global::dataPath().append(QStringLiteral("/libZhiNengTiKaQML_arm64-v8a.so")));
    if(!libraryFile.exists())
    {
        QEventLoop eventLoop;
        LibraryUpdateChecker libraryUpdateChecker(QStringLiteral("0.0.0"), &a);
        QObject::connect(&libraryUpdateChecker, &LibraryUpdateChecker::checkFinished, &libraryUpdateChecker, &LibraryUpdateChecker::downloadNewestVersion);
        QObject::connect(&libraryUpdateChecker, &LibraryUpdateChecker::downloadFinished, [&eventLoop]
        {
            QMessageBox::information(nullptr, {}, QStringLiteral("下载主程序动态库文件成功"));
            eventLoop.quit();
        });
        libraryUpdateChecker.start();
        eventLoop.exec();
    }
    QFile newVersionLibraryFile(Global::dataPath().append(QStringLiteral("/newVersionLibrary")));
    if(newVersionLibraryFile.exists())
    {
        libraryFile.remove();
        newVersionLibraryFile.copy(Global::dataPath().append(QStringLiteral("/libZhiNengTiKaQML_arm64-v8a.so")));
        newVersionLibraryFile.remove();
    }


    QLibrary library(Global::dataPath().append(QStringLiteral("/libZhiNengTiKaQML_arm64-v8a")));
    if(!library.load())
    {
        QMessageBox::critical(nullptr, {}, QStringLiteral("load library failed\n")
                              .append(library.errorString()));
    }
    else
    {
        LibraryUpdateChecker *libraryUpdateChecker;

        typedef const char * (*GetZhiNengTiKaQMLVersion)();
        GetZhiNengTiKaQMLVersion getZhiNengTiKaQMLVersion = (GetZhiNengTiKaQMLVersion)library.resolve("getVersion");

        if(getZhiNengTiKaQMLVersion)
        {
            const QString currentVersion(getZhiNengTiKaQMLVersion());
            qDebug() << "Library Version:" << currentVersion;
            libraryUpdateChecker = new LibraryUpdateChecker(currentVersion, &a);
            QObject::connect(libraryUpdateChecker, &LibraryUpdateChecker::checkFinishedAndHasNewVersion, libraryUpdateChecker, &LibraryUpdateChecker::downloadNewestVersion);
            QObject::connect(libraryUpdateChecker, &LibraryUpdateChecker::downloadFinished, []
            {
                QMessageBox::information(nullptr, {}, QStringLiteral("下载主程序动态库文件成功, 将在下次启动时自动更新"));
            });
            libraryUpdateChecker->start();
        }

        typedef int (*ZhiNengTiKaQML)(QApplication *);
        ZhiNengTiKaQML zhiNengTiKaQML = (ZhiNengTiKaQML)library.resolve("run");

        if(zhiNengTiKaQML)
        {
            auto result(zhiNengTiKaQML(&a));
            if(libraryUpdateChecker)
            {
                libraryUpdateChecker->quit();
                libraryUpdateChecker->wait();
            }
            return result;
        }
    }
    return -1;
}

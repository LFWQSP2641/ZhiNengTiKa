#include "LibraryUpdateChecker.h"
#include "../StaticClass/Global.h"

LibraryUpdateChecker::LibraryUpdateChecker(QObject *parent)
    : LibraryUpdateChecker{Version(), parent}
{
}

LibraryUpdateChecker::LibraryUpdateChecker(const Version &currentVersion, QObject *parent)
    : UpdateChecker(currentVersion, parent),
      domain(QStringLiteral(DATABASE_DOMAIN).append(QStringLiteral("LibraryUpdate/"))),
      saveFilePath(Global::tempPath().append(QStringLiteral("/newVersionLibrary")))
{
}

void LibraryUpdateChecker::installNewestVersion()
{
    qDebug() << Q_FUNC_INFO;
}

QString LibraryUpdateChecker::getDomain() const
{
    return domain;
}

QString LibraryUpdateChecker::getSaveFilePath() const
{
    return saveFilePath;
}

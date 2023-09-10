#include "GUI/MainWidget.h"
#include "Logic/UpdateChecker.h"
#include "StaticClass/Global.h"
#include "Singleton/Settings.h"
#include "Singleton/Network.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Network::initOnce();

    Global::initOnce();
    Global::deleteDir(Global::dataPath().append(QStringLiteral("/Image")));

    a.setWindowIcon(QIcon(QStringLiteral(":/ico/xinjiaoyuico.png")));
    a.setApplicationDisplayName(QStringLiteral("智能题卡"));

    Settings::initOnce();
    auto settings(Settings::getSingletonSettings());
    UserData::initPublicUserData();

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

    MainWidget w;
    w.setEnabled(false);
    w.show();
    if(UpdateChecker().checkMinimumVersion())
    {
        w.setEnabled(true);
    }
    else
    {
        QMessageBox::critical(&w, QStringLiteral("版本过低"), QStringLiteral("请更新版本或检查网络连接"));
        a.exit(1);
        return 1;
    }

    return a.exec();
}

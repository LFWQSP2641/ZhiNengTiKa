#include "GUI/MainWidget.h"
#include "Logic/UpdateChecker.h"
#include "StaticClass/Global.h"
#include "StaticClass/Setting.h"
#include "Singleton/Network.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Network::initOnce();

    Global::initOnce();

    a.setWindowIcon(QIcon(QStringLiteral(":/ico/xinjiaoyuico.png")));
    a.setApplicationDisplayName(QStringLiteral("智能题卡"));

    Setting::loadFromFile();
    UserData::initPublicUserData();

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

#include "GUI/MainWidget.h"
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
    w.show();

    return a.exec();
}

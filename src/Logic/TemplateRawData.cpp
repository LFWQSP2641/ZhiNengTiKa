#include "TemplateRawData.h"
#include "../StaticClass/Global.h"
#include "../StaticClass/XinjiaoyuNetwork.h"

TemplateRawData::TemplateRawData(const TemplateSummary &templateSummary)
    : TemplateSummary(templateSummary)
{
#ifdef Q_OS_ANDROID
    QFile file(QStringLiteral("assets:/templateData/").append(templateCode));
#else
    QFile file(QStringLiteral(":/templateData/").append(templateCode));
#endif
    QFile fileTemp { Global::dataPath().append(QStringLiteral("/TemplateFile/")).append(templateCode) };
    if (file.exists())
    {
        this->internal = true;
        file.open(QFile::ReadOnly);
        rawData = file.readAll();
        file.close();
    }
    else if (fileTemp.exists())
    {
        this->external = true;
        fileTemp.open(QFile::ReadOnly);
        rawData = fileTemp.readAll();
        fileTemp.close();
    }
    else
    {
        this->network = true;
        rawData = XinjiaoyuNetwork::getTemplateCodeData(templateCode);
        if (rawData.isEmpty())
        {
            errorStr = QStringLiteral("请求题卡后,返回值为空\n"
                                      "请检查登录状态和网络");
            return;
        }
        else if(!rawData.startsWith("{\"code\":200,"))
        {
            errorStr = QStringLiteral("服务器报错\n"
                                      "请尝试在设置中登陆你所在学校, 同一学年的账号\n"
                                      "返回结果:%1").arg(rawData);
            return;
        }
        fileTemp.open(QFile::WriteOnly);
        fileTemp.write(rawData);
        fileTemp.close();
    }
    this->valid = true;
}

TemplateRawData::TemplateRawData(const QString &templateCode)
    : TemplateRawData(TemplateSummary(QString(), templateCode))
{
    templateName = QJsonDocument::fromJson(rawData).object().value(QStringLiteral("data")).toObject().value(QStringLiteral("templateName")).toString();
}


bool TemplateRawData::isInternal() const
{
    return internal;
}

bool TemplateRawData::isExternal() const
{
    return external;
}

bool TemplateRawData::isNetwork() const
{
    return network;
}

bool TemplateRawData::isValid() const
{
    return valid;
}

QString TemplateRawData::getErrorStr() const
{
    return errorStr;
}

QByteArray TemplateRawData::getRawData() const
{
    return rawData;
}

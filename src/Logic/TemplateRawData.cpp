#include "TemplateRawData.h"
#include "../StaticClass/Global.h"
#include "../StaticClass/XinjiaoyuNetwork.h"

TemplateRawData::TemplateRawData(const TemplateSummary &templateSummary)
    : TemplateSummary(templateSummary)
{
#ifdef Q_OS_ANDROID
    QFile file { QStringLiteral("assets:/templateData/") + templateCode };
#else
    QFile file { QStringLiteral(":/templateData/") + templateCode };
#endif
    QFile fileTemp { Global::tempPath() + QDir::separator() + QStringLiteral("TemplateFile") + QDir::separator() + templateCode };
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
        try
        {
            rawData = XinjiaoyuNetwork::getTemplateCodeData(templateCode);
        }
        catch (const std::exception &e)
        {
            errorStr = e.what();
            return;
        }
        this->network = true;
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

#include "WebRawData.h"
#include "../StaticClass/Global.h"
#include "../StaticClass/XinjiaoyuNetwork.h"

WebRawData::WebRawData(const QString &templateCode)
{
    this->templateCode = templateCode;
#ifdef Q_OS_ANDROID
    QFile file { QStringLiteral("assets:/templateData/") + templateCode };
#else
    QFile file { QStringLiteral(":/template/templateData/") + templateCode };
#endif
    QFile fileTemp { Global::tempPath() + QDir::separator() + QStringLiteral("TemplateFile") + QDir::separator() + templateCode };
    if (file.exists())
    {
        this->internal = true;
        file.open(QFile::ReadOnly);
        rawData = file.readAll();
        templateName = QJsonDocument::fromJson(rawData).object().value(QStringLiteral("data")).toObject().value(QStringLiteral("templateName")).toString();
        file.close();
    }
    else if (fileTemp.exists())
    {
        this->external = true;
        fileTemp.open(QFile::ReadOnly);
        rawData = fileTemp.readAll();
        fileTemp.close();
        templateName = QJsonDocument::fromJson(rawData).object().value(QStringLiteral("data")).toObject().value(QStringLiteral("templateName")).toString();
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
        templateName = QJsonDocument::fromJson(rawData).object().value(QStringLiteral("data")).toObject().value(QStringLiteral("templateName")).toString();
    }
    this->valid = true;
}

bool WebRawData::getInternal() const
{
    return internal;
}

bool WebRawData::getExternal() const
{
    return external;
}

bool WebRawData::getNetwork() const
{
    return network;
}

bool WebRawData::getValid() const
{
    return valid;
}

QString WebRawData::getErrorStr() const
{
    return errorStr;
}

AnalysisWebRawData WebRawData::toAnalysisWebRawData() const
{
    return AnalysisWebRawData(this->rawData, this->templateName, this->templateCode);
}

QByteArray WebRawData::getRawData() const
{
    return rawData;
}

QString WebRawData::getTemplateCode() const
{
    return templateCode;
}

QString WebRawData::getTemplateName() const
{
    return templateName;
}


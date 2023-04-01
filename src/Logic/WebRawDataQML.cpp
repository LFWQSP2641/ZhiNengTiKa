#include "WebRawDataQML.h"
#include "../StaticClass/Global.h"
#include "../StaticClass/XinjiaoyuNetwork.h"

WebRawDataQML::WebRawDataQML(const QString &templateCode)
{
    setValue(templateCode);
}

void WebRawDataQML::setValue(const QString &templateCode)
{
    this->templateCode = templateCode;
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
            this->valid = false;
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

bool WebRawDataQML::getInternal() const
{
    return internal;
}

bool WebRawDataQML::getExternal() const
{
    return external;
}

bool WebRawDataQML::getNetwork() const
{
    return network;
}

bool WebRawDataQML::getValid() const
{
    return valid;
}

QString WebRawDataQML::getErrorStr() const
{
    return errorStr;
}

AnalysisWebRawDataQML *WebRawDataQML::toAnalysisWebRawDataQML()
{
    if(this->newAnalysisWebRawDataQMLPointer != nullptr)
    {
        this->newAnalysisWebRawDataQMLPointer->deleteLater();
    }
    this->newAnalysisWebRawDataQMLPointer = new AnalysisWebRawDataQML(this->rawData, this->templateName, this->templateCode);
    return this->newAnalysisWebRawDataQMLPointer;
}

QByteArray WebRawDataQML::getRawData() const
{
    return rawData;
}

QString WebRawDataQML::getTemplateCode() const
{
    return templateCode;
}

QString WebRawDataQML::getTemplateName() const
{
    return templateName;
}


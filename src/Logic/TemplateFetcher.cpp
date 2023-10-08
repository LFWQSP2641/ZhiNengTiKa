#include "TemplateFetcher.h"
#include "../StaticClass/Global.h"
#include "../StaticClass/XinjiaoyuNetwork.h"

TemplateFetcher::TemplateFetcher(QObject *parent)
    : QObject{parent}
{

}

void TemplateFetcher::handleTemplateRequest(TemplateSummary *templateSummary)
{
    const auto templateCode(templateSummary->getTemplateCode());
    auto *templateAnalysis(new TemplateAnalysis(this));
    templateAnalysis->setTemplateCode(templateCode);
    templateAnalysis->setTemplateName(templateSummary->getTemplateName());
    templateAnalysis->setVolume(templateSummary->getVolume());
    templateAnalysis->setSubject(templateSummary->getSubject());
#ifdef Q_OS_ANDROID
    QFile file(QStringLiteral("assets:/templateData/").append(templateCode));
#else
    QFile file(QStringLiteral(":/templateData/").append(templateCode));
#endif
    QFile fileTemp { Global::dataPath().append(QStringLiteral("/TemplateFile/")).append(templateCode) };
    if (file.exists())
    {
        templateAnalysis->internal = true;
        file.open(QFile::ReadOnly);
        QByteArray rawData(file.readAll());
        file.close();
        templateAnalysis->analyze(rawData);
        emit templateAnalysisReady(templateAnalysis);
    }
    else if (fileTemp.exists())
    {
        templateAnalysis->external = true;
        fileTemp.open(QFile::ReadOnly);
        QByteArray rawData(fileTemp.readAll());
        fileTemp.close();
        templateAnalysis->analyze(rawData);
        emit templateAnalysisReady(templateAnalysis);
    }
    else
    {
        this->templateAnalysisfromNetwork = templateAnalysis;
        templateAnalysis->network = true;
        auto reply(XinjiaoyuNetwork::getTemplateCodeData(templateCode));
        connect(reply, &QNetworkReply::finished, this, &TemplateFetcher::onHandleTemplateReplyFinished);
    }
}

void TemplateFetcher::handleTemplateRequestByCode(const QString &templateCode)
{
    TemplateSummary templateSummary(QString(), templateCode);
    handleTemplateRequest(&templateSummary);
}

void TemplateFetcher::onHandleTemplateReplyFinished()
{
    auto reply(qobject_cast<QNetworkReply *>(sender()));
    auto rawData(reply->readAll());
    if (rawData.isEmpty())
    {
        emit error(QStringLiteral("请求题卡后,返回值为空\n"
                                  "请检查网络状态"));
        this->templateAnalysisfromNetwork->deleteLater();
        return;
    }
    else if(!rawData.startsWith("{\"code\":200,"))
    {
        emit error(QStringLiteral("服务器报错\n"
                                  "请尝试在设置中登陆你所在学校, 同一学年的账号\n"
                                  "返回结果:\n%1").arg(rawData));
        this->templateAnalysisfromNetwork->deleteLater();
        return;
    }
    auto templateData(XinjiaoyuNetwork::decodeTemplateReply(rawData));
    QFile fileTemp { Global::dataPath().append(QStringLiteral("/TemplateFile/")).append(templateAnalysisfromNetwork->templateCode) };
    fileTemp.open(QFile::WriteOnly);
    fileTemp.write(templateData);
    fileTemp.close();
    templateAnalysisfromNetwork->analyze(templateData);
    if(templateAnalysisfromNetwork->templateName.isEmpty())
    {
        templateAnalysisfromNetwork->templateName = QJsonDocument::fromJson(templateData).object().value(QStringLiteral("data")).toObject().value(QStringLiteral("templateName")).toString();
    }
    emit templateAnalysisReady(templateAnalysisfromNetwork);
}

#include "TemplateSummary.h"

QString TemplateSummary::getTemplateName() const
{
    return templateName;
}

void TemplateSummary::setTemplateName(const QString &newTemplateName)
{
    if (templateName == newTemplateName)
        return;
    templateName = newTemplateName;
}

QString TemplateSummary::getTemplateCode() const
{
    return templateCode;
}

void TemplateSummary::setTemplateCode(const QString &newTemplateCode)
{
    if (templateCode == newTemplateCode)
        return;
    templateCode = newTemplateCode;
}

QString TemplateSummary::getVolume() const
{
    return volume;
}

void TemplateSummary::setVolume(const QString &newVolume)
{
    if (volume == newVolume)
        return;
    volume = newVolume;
}

QString TemplateSummary::getSubject() const
{
    return subject;
}

void TemplateSummary::setSubject(const QString &newSubject)
{
    if (subject == newSubject)
        return;
    subject = newSubject;
}

QDebug operator<<(QDebug dbg, const TemplateSummary &templateSummary)
{
    dbg.space() << QStringLiteral("TemplateName:") << templateSummary.getTemplateName()
                << QStringLiteral("TemplateCode:") << templateSummary.getTemplateCode();
    return dbg.maybeSpace();
}

#include "TemplateSummary.h"

TemplateSummary::TemplateSummary(const QString &templateName,
                                 const QString &templateCode,
                                 const QString &volume, const QString &subject)
    : templateName(templateName), templateCode(templateCode), volume(volume),
      subject(subject) {}

QString TemplateSummary::getTemplateName() const
{
    return templateName;
}

void TemplateSummary::setTemplateName(const QString &newTemplateName)
{
    templateName = newTemplateName;
}

QString TemplateSummary::getTemplateCode() const
{
    return templateCode;
}

void TemplateSummary::setTemplateCode(const QString &newTemplateCode)
{
    templateCode = newTemplateCode;
}

QString TemplateSummary::getVolume() const
{
    return volume;
}

void TemplateSummary::setVolume(const QString &newVolume)
{
    volume = newVolume;
}

QString TemplateSummary::getSubject() const
{
    return subject;
}

void TemplateSummary::setSubject(const QString &newSubject)
{
    subject = newSubject;
}

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
    emit templateNameChanged();
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
    emit templateCodeChanged();
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
    emit volumeChanged();
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
    emit subjectChanged();
}

#include "TemplateSummaryQML.h"

TemplateSummaryQML::TemplateSummaryQML(QObject *parent)
    : QObject{parent}
{

}

TemplateSummaryQML::TemplateSummaryQML(const QString &templateName, const QString &templateCode, const QString &volume, const QString &subject, QObject *parent)
    : QObject{parent}, templateSummary(templateName, templateCode, volume, subject)
{

}

TemplateSummaryQML::TemplateSummaryQML(const TemplateSummary &other, QObject *parent)
    : QObject{parent}, templateSummary(other)
{

}

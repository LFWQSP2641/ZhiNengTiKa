#include "TemplateRawDataQML.h"
#include "TemplateSummaryQML.h"

TemplateRawDataQML::TemplateRawDataQML(QObject *parent)
    : QObject{parent}
{

}

TemplateRawDataQML::TemplateRawDataQML(const TemplateSummary &templateSummary, QObject *parent)
    : QObject{parent}, templateRawData(templateSummary)
{

}

TemplateRawDataQML::TemplateRawDataQML(const TemplateSummaryQML &templateSummaryQML, QObject *parent)
    : QObject{parent}, templateRawData(templateSummaryQML.getTemplateSummary())
{

}

TemplateRawDataQML::TemplateRawDataQML(const QString &templateCode, QObject *parent)
    : QObject{parent}, templateRawData(templateCode)
{

}

TemplateRawDataQML::TemplateRawDataQML(const TemplateRawData &other, QObject *parent)
    : QObject{parent}, templateRawData(other)
{

}

void TemplateRawDataQML::setValue(const QString &templateCode)
{
    this->templateRawData = TemplateRawData(templateCode.trimmed());
}

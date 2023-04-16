#include "TemplateAnalysisQML.h"
#include "TemplateRawDataQML.h"

TemplateAnalysisQML::TemplateAnalysisQML(QObject *parent)
    : QObject{parent}
{

}

TemplateAnalysisQML::TemplateAnalysisQML(const TemplateRawData &templateRawData, QObject *parent)
    : QObject{parent}, templateAnalysis(templateRawData)
{

}

TemplateAnalysisQML::TemplateAnalysisQML(const TemplateRawDataQML &templateRawDataQML, QObject *parent)
    : QObject{parent}, templateAnalysis(templateRawDataQML.getTemplateRawData())
{

}

void TemplateAnalysisQML::setValue(TemplateRawDataQML *templateRawDataQML)
{
    this->templateAnalysis = TemplateAnalysis(templateRawDataQML->getTemplateRawData());
}


#ifndef TEMPLATERAWDATAQML_H
#define TEMPLATERAWDATAQML_H

#include "../Logic/TemplateRawData.h"

class TemplateSummaryQML;

class TemplateRawDataQML : public QObject
{
    Q_OBJECT
public:
    explicit TemplateRawDataQML(QObject *parent = nullptr);
    explicit TemplateRawDataQML(const TemplateSummary &templateSummary, QObject *parent = nullptr);
    explicit TemplateRawDataQML(const TemplateSummaryQML &templateSummaryQML, QObject *parent = nullptr);
    explicit TemplateRawDataQML(const QString &templateCode, QObject *parent = nullptr);
    explicit TemplateRawDataQML(const TemplateRawData &other, QObject *parent = nullptr);

    Q_INVOKABLE void setValue(const QString &templateCode);

    Q_INVOKABLE TemplateRawData getTemplateRawData() const
    {
        return templateRawData;
    }

    // TemplateRawData function start
    Q_INVOKABLE bool isInternal() const
    {
        return templateRawData.isInternal();
    }
    Q_INVOKABLE bool isExternal() const
    {
        return templateRawData.isExternal();
    }
    Q_INVOKABLE bool isNetwork() const
    {
        return templateRawData.isNetwork();
    }
    Q_INVOKABLE bool isValid() const
    {
        return templateRawData.isValid();
    }
    Q_INVOKABLE QString getErrorStr() const
    {
        return templateRawData.getErrorStr();
    }
    Q_INVOKABLE QByteArray getRawData() const
    {
        return templateRawData.getRawData();
    }
    // TemplateRawData function end

    // TemplateSummary function start
    Q_INVOKABLE QString getTemplateName() const
    {
        return templateRawData.getTemplateName();
    }
    Q_INVOKABLE void setTemplateName(const QString &newTemplateName)
    {
        templateRawData.setTemplateName(newTemplateName);
    }

    Q_INVOKABLE QString getTemplateCode() const
    {
        return templateRawData.getTemplateCode();
    }
    Q_INVOKABLE void setTemplateCode(const QString &newTemplateCode)
    {
        templateRawData.setTemplateCode(newTemplateCode);
    }

    Q_INVOKABLE QString getVolume() const
    {
        return templateRawData.getVolume();
    }
    Q_INVOKABLE void setVolume(const QString &newVolume)
    {
        templateRawData.setVolume(newVolume);
    }

    Q_INVOKABLE QString getSubject() const
    {
        return templateRawData.getSubject();
    }
    Q_INVOKABLE void setSubject(const QString &newSubject)
    {
        templateRawData.setSubject(newSubject);
    }
    // TemplateSummary function end

protected:
    TemplateRawData templateRawData;
};

#endif // TEMPLATERAWDATAQML_H

#ifndef TEMPLATESUMMARYQML_H
#define TEMPLATESUMMARYQML_H

#include "../Logic/TemplateSummary.h"

class TemplateSummaryQML : public QObject
{
    Q_OBJECT
public:
    explicit TemplateSummaryQML(QObject *parent = nullptr);
    explicit TemplateSummaryQML(const QString &templateName,
                                const QString &templateCode,
                                const QString &volume = QString(),
                                const QString &subject = QString(),
                                QObject *parent = nullptr);

    explicit TemplateSummaryQML(const TemplateSummary &other,
                                QObject *parent = nullptr);

    Q_INVOKABLE TemplateSummary getTemplateSummary() const
    {
        return templateSummary;
    }

    Q_INVOKABLE QString getTemplateName() const
    {
        return templateSummary.getTemplateName();
    }
    Q_INVOKABLE void setTemplateName(const QString &newTemplateName)
    {
        templateSummary.setTemplateName(newTemplateName);
    }

    Q_INVOKABLE QString getTemplateCode() const
    {
        return templateSummary.getTemplateCode();
    }
    Q_INVOKABLE void setTemplateCode(const QString &newTemplateCode)
    {
        templateSummary.setTemplateCode(newTemplateCode);
    }

    Q_INVOKABLE QString getVolume() const
    {
        return templateSummary.getVolume();
    }
    Q_INVOKABLE void setVolume(const QString &newVolume)
    {
        templateSummary.setVolume(newVolume);
    }

    Q_INVOKABLE QString getSubject() const
    {
        return templateSummary.getSubject();
    }
    Q_INVOKABLE void setSubject(const QString &newSubject)
    {
        templateSummary.setSubject(newSubject);
    }

protected:
    TemplateSummary templateSummary;
};

#endif // TEMPLATESUMMARYQML_H

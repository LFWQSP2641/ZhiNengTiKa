#ifndef TEMPLATESUMMARY_H
#define TEMPLATESUMMARY_H


class TemplateSummary
{
public:
    TemplateSummary() = default;
    TemplateSummary(const QString &templateName, const QString &templateCode,
                    const QString &volume = QString(),
                    const QString &subject = QString());

    QString getTemplateName() const;
    void setTemplateName(const QString &newTemplateName);

    QString getTemplateCode() const;
    void setTemplateCode(const QString &newTemplateCode);

    QString getVolume() const;
    void setVolume(const QString &newVolume);

    QString getSubject() const;
    void setSubject(const QString &newSubject);

protected:
    QString templateName;
    QString templateCode;
    QString volume;
    QString subject;
};

Q_DECLARE_METATYPE(TemplateSummary)

#endif // TEMPLATESUMMARY_H

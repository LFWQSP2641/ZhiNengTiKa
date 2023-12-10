#ifndef TEMPLATESUMMARY_H
#define TEMPLATESUMMARY_H

class TemplateFetcher;

class TemplateSummary
{
    Q_GADGET
    friend class TemplateFetcher;
public:
    explicit TemplateSummary() = default;
    TemplateSummary(const QString &templateName, const QString &templateCode,
                    const QString &volume = QString(),
                    const QString &subject = QString())
        : templateName(templateName), templateCode(templateCode),
          volume(volume), subject(subject) {}

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
private:
    Q_PROPERTY(QString templateName READ getTemplateName WRITE setTemplateName FINAL)
    Q_PROPERTY(QString templateCode READ getTemplateCode WRITE setTemplateCode FINAL)
    Q_PROPERTY(QString volume READ getVolume WRITE setVolume FINAL)
    Q_PROPERTY(QString subject READ getSubject WRITE setSubject FINAL)
};

Q_DECLARE_METATYPE(TemplateSummary)
QDebug operator<<(QDebug dbg, const TemplateSummary &templateSummary);

#endif // TEMPLATESUMMARY_H

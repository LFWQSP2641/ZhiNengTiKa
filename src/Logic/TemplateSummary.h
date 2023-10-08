#ifndef TEMPLATESUMMARY_H
#define TEMPLATESUMMARY_H

class TemplateFetcher;

class TemplateSummary : public QObject
{
    Q_OBJECT
    friend class TemplateFetcher;
public:
    explicit TemplateSummary(QObject *parent = nullptr)
        : QObject{parent} {}
    TemplateSummary(const QString &templateName, const QString &templateCode,
                    const QString &volume = QString(),
                    const QString &subject = QString(),
                    QObject *parent = nullptr)
        : QObject{parent}, templateName(templateName),
          templateCode(templateCode), volume(volume), subject(subject) {}

    QString getTemplateName() const;
    void setTemplateName(const QString &newTemplateName);

    QString getTemplateCode() const;
    void setTemplateCode(const QString &newTemplateCode);

    QString getVolume() const;
    void setVolume(const QString &newVolume);

    QString getSubject() const;
    void setSubject(const QString &newSubject);

signals:
    void templateNameChanged();

    void templateCodeChanged();

    void volumeChanged();

    void subjectChanged();

protected:
    QString templateName;
    QString templateCode;
    QString volume;
    QString subject;
private:
    Q_PROPERTY(QString templateName READ getTemplateName WRITE setTemplateName NOTIFY templateNameChanged FINAL)
    Q_PROPERTY(QString templateCode READ getTemplateCode WRITE setTemplateCode NOTIFY templateCodeChanged FINAL)
    Q_PROPERTY(QString volume READ getVolume WRITE setVolume NOTIFY volumeChanged FINAL)
    Q_PROPERTY(QString subject READ getSubject WRITE setSubject NOTIFY subjectChanged FINAL)
};

#endif // TEMPLATESUMMARY_H

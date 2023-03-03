#ifndef TEMPLATESEARCHER_H
#define TEMPLATESEARCHER_H

class TemplateSearcher : public QThread
{
    Q_OBJECT
public:
    explicit TemplateSearcher(QObject *parent = nullptr);
    void setSearchedTemplateName(const QString &searcheredTemplateName);

public slots:
    void stop();

protected:
    volatile bool toStop = false;

    QString templateName;
    void run() override;

signals:
    void searchResult(const QString &templateName, const QString &templateCode);
    void searchFinished(bool success);
    void searchStoped();
};

#endif // TEMPLATESEARCHER_H

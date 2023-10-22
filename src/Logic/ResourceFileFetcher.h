#ifndef RESOURCEFILEFETCHER_H
#define RESOURCEFILEFETCHER_H

#include "ResourceFileModel.h"

class ResourceFileFetcher : public QObject
{
    Q_OBJECT
public:
    explicit ResourceFileFetcher(QObject *parent = nullptr);

    ResourceFileModel *getModel() const;

    Q_INVOKABLE QVariant getSubjectModel();
    Q_INVOKABLE QVariant getEditionModel(const QString &subject);
    Q_INVOKABLE QVariant getModuleModel(const QString &subject, const QString &edition);
    Q_INVOKABLE bool haveNextPage() const;

public slots:
    void init();
    void continueLoadModel();
    void resetModel(const QString &subject, const QString &edition, const QString &module);

protected:
    static QNetworkRequest setRequest(const QUrl &url);
    static QString generateRandomString(qsizetype size = 1);

    QJsonObject catalogDetail;

    ResourceFileModel *model;

    QString currentBookCatalogId;
    int currentPage = 1;
    int maxPage = 1;

protected slots:
    void onCatalogArrayReplyFinished();
    void onCatalogDetailReplyFinished();
    void onCatalogReplyFinished();
    void onResourceReplyFinished();
    void onContinueLoadResourceReplyFinished();

signals:
    void error(const QString &msg);
    void initFinished();
    void resetModelFinished();
    void continueLoadModelFinished();
private:
    Q_PROPERTY(ResourceFileModel *model READ getModel CONSTANT FINAL)
};

#endif // RESOURCEFILEFETCHER_H

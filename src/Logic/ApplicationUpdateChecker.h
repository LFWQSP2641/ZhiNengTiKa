#ifndef APPLICATIONUPDATECHECKER_H
#define APPLICATIONUPDATECHECKER_H

#include "UpdateChecker.h"

class ApplicationUpdateChecker : public UpdateChecker
{
    Q_OBJECT
public:
    explicit ApplicationUpdateChecker(QObject *parent = nullptr);
public slots:
    void installNewestVersion() override;
protected:
    QString getDomain() const override;
    QString getSaveFilePath() const override;

    const QString domain;
    const QString saveFilePath;
private:
    Q_PROPERTY(QString domain READ getDomain CONSTANT FINAL)
    Q_PROPERTY(QString saveFilePath READ getSaveFilePath CONSTANT FINAL)
};

#endif // APPLICATIONUPDATECHECKER_H

#ifndef TEMPLATERAWDATA_H
#define TEMPLATERAWDATA_H

#include "TemplateSummary.h"

class TemplateRawData : public TemplateSummary
{
public:
    TemplateRawData() = default;
    explicit TemplateRawData(const TemplateSummary &templateSummary);
    explicit TemplateRawData(const QString &templateCode);

    bool isInternal() const;

    bool isExternal() const;

    bool isNetwork() const;

    bool isValid() const;

    QString getErrorStr() const;

    QByteArray getRawData() const;

protected:
    bool internal = false;
    bool external = false;
    bool network = false;
    bool valid = false;
    QByteArray rawData;
    QString errorStr;
};

Q_DECLARE_METATYPE(TemplateRawData)

#endif // TEMPLATERAWDATA_H

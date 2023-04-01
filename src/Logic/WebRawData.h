#ifndef WEBRAWDATA_H
#define WEBRAWDATA_H

#include "AnalysisWebRawData.h"

class WebRawData
{
public:
    explicit WebRawData(const QString &templateCode);

    bool getInternal() const;

    bool getExternal() const;

    bool getNetwork() const;

    bool getValid() const;

    QString getErrorStr() const;

    QByteArray getRawData() const;

    QString getTemplateCode() const;

    QString getTemplateName() const;


    AnalysisWebRawData toAnalysisWebRawData() const;
    operator AnalysisWebRawData()
    {
        return toAnalysisWebRawData();
    };

protected:
    bool internal = false;
    bool external = false;
    bool network = false;
    bool valid = false;
    QByteArray rawData;
    QString templateName;
    QString templateCode;
    QString errorStr;
};

#endif // WEBRAWDATA_H

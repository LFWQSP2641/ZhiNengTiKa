#ifndef WEBRAWDATA_H
#define WEBRAWDATA_H

#include "AnalysisWebRawDataQML.h"

class WebRawDataQML: public QObject
{
    Q_OBJECT
public:
    WebRawDataQML() = default;
    explicit WebRawDataQML(const QString &templateCode);

    Q_INVOKABLE void setValue(const QString &templateCode);

    Q_INVOKABLE bool getInternal() const;

    Q_INVOKABLE bool getExternal() const;

    Q_INVOKABLE bool getNetwork() const;

    Q_INVOKABLE bool getValid() const;

    Q_INVOKABLE QString getErrorStr() const;

    Q_INVOKABLE QByteArray getRawData() const;

    Q_INVOKABLE QString getTemplateCode() const;

    Q_INVOKABLE QString getTemplateName() const;


    Q_INVOKABLE AnalysisWebRawDataQML *toAnalysisWebRawDataQML();

protected:
    bool internal = false;
    bool external = false;
    bool network = false;
    bool valid = false;
    QByteArray rawData;
    QString templateName;
    QString templateCode;
    QString errorStr;

    AnalysisWebRawDataQML *newAnalysisWebRawDataQMLPointer = nullptr;
};

#endif // WEBRAWDATA_H

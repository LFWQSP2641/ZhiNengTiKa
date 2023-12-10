#ifndef ZXINGRESULT_H
#define ZXINGRESULT_H

#include <QObject>
#include "ZXingCpp_global.h"
#include "ZXingPosition.h"

class ZXINGCPP_EXPORT ZXingResult
{
    Q_GADGET
public:
    ZXingResult() = default;

    QString getText() const;
    void setText(const QString &newText);

    ZXingPosition getPosition() const;
    void setPosition(const ZXingPosition &newPosition);

protected:
    QString text;
    ZXingPosition position;

private:
    Q_PROPERTY(QString text READ getText WRITE setText FINAL)
    Q_PROPERTY(ZXingPosition position READ getPosition WRITE setPosition FINAL)
};

Q_DECLARE_METATYPE(ZXingResult)

#endif // ZXINGRESULT_H

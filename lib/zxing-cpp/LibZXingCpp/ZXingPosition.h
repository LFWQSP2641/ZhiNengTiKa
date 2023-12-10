#ifndef ZXINGPOSITION_H
#define ZXINGPOSITION_H

#include <QObject>
#include <QPoint>
#include "ZXingCpp_global.h"

class ZXINGCPP_EXPORT ZXingPosition
{
    Q_GADGET
public:
    ZXingPosition() = default;

    QPoint getTopLeft() const;
    void setTopLeft(QPoint newTopLeft);

    QPoint getTopRight() const;
    void setTopRight(QPoint newTopRight);

    QPoint getBottomRight() const;
    void setBottomRight(QPoint newBottomRight);

    QPoint getBottomLeft() const;
    void setBottomLeft(QPoint newBottomLeft);

    bool operator==(const ZXingPosition &other) const;

protected:
    QPoint topLeft;
    QPoint topRight;
    QPoint bottomRight;
    QPoint bottomLeft;

private:
    Q_PROPERTY(QPoint topLeft READ getTopLeft WRITE setTopLeft FINAL)
    Q_PROPERTY(QPoint topRight READ getTopRight WRITE setTopRight FINAL)
    Q_PROPERTY(QPoint bottomRight READ getBottomRight WRITE setBottomRight FINAL)
    Q_PROPERTY(QPoint bottomLeft READ getBottomLeft WRITE setBottomLeft FINAL)
};

Q_DECLARE_METATYPE(ZXingPosition)

#endif // ZXINGPOSITION_H

#ifndef ZXINGPOSITION_H
#define ZXINGPOSITION_H

#include <QObject>
#include <QPoint>
#include "ZXingCpp_global.h"

class ZXINGCPP_EXPORT ZXingPosition : public QObject
{
    Q_OBJECT
public:
    explicit ZXingPosition(QObject *parent = nullptr);

    QPoint getTopLeft() const;
    void setTopLeft(QPoint newTopLeft);

    QPoint getTopRight() const;
    void setTopRight(QPoint newTopRight);

    QPoint getBottomRight() const;
    void setBottomRight(QPoint newBottomRight);

    QPoint getBottomLeft() const;
    void setBottomLeft(QPoint newBottomLeft);

protected:
    QPoint topLeft;
    QPoint topRight;
    QPoint bottomRight;
    QPoint bottomLeft;

signals:

    void topLeftChanged();
    void topRightChanged();

    void bottomRightChanged();

    void bottomLeftChanged();

private:
    Q_PROPERTY(QPoint topLeft READ getTopLeft WRITE setTopLeft NOTIFY topLeftChanged FINAL)
    Q_PROPERTY(QPoint topRight READ getTopRight WRITE setTopRight NOTIFY topRightChanged FINAL)
    Q_PROPERTY(QPoint bottomRight READ getBottomRight WRITE setBottomRight NOTIFY bottomRightChanged FINAL)
    Q_PROPERTY(QPoint bottomLeft READ getBottomLeft WRITE setBottomLeft NOTIFY bottomLeftChanged FINAL)
};

#endif // ZXINGPOSITION_H

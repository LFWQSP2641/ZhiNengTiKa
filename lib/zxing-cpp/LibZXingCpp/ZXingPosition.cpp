#include "ZXingPosition.h"

ZXingPosition::ZXingPosition(QObject *parent)
    : QObject{parent}
{

}

QPoint ZXingPosition::getTopLeft() const
{
    return topLeft;
}

void ZXingPosition::setTopLeft(QPoint newTopLeft)
{
    if (topLeft == newTopLeft)
        return;
    topLeft = newTopLeft;
    emit topLeftChanged();
}

QPoint ZXingPosition::getTopRight() const
{
    return topRight;
}

void ZXingPosition::setTopRight(QPoint newTopRight)
{
    if (topRight == newTopRight)
        return;
    topRight = newTopRight;
    emit topRightChanged();
}

QPoint ZXingPosition::getBottomRight() const
{
    return bottomRight;
}

void ZXingPosition::setBottomRight(QPoint newBottomRight)
{
    if (bottomRight == newBottomRight)
        return;
    bottomRight = newBottomRight;
    emit bottomRightChanged();
}

QPoint ZXingPosition::getBottomLeft() const
{
    return bottomLeft;
}

void ZXingPosition::setBottomLeft(QPoint newBottomLeft)
{
    if (bottomLeft == newBottomLeft)
        return;
    bottomLeft = newBottomLeft;
    emit bottomLeftChanged();
}

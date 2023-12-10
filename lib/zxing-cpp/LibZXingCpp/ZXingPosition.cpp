#include "ZXingPosition.h"

QPoint ZXingPosition::getTopLeft() const
{
    return topLeft;
}

void ZXingPosition::setTopLeft(QPoint newTopLeft)
{
    if (topLeft == newTopLeft)
        return;
    topLeft = newTopLeft;
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
}

bool ZXingPosition::operator==(const ZXingPosition &other) const
{
    return ((this->topLeft == other.topLeft) &&
            (this->topRight == other.topRight) &&
            (this->bottomRight == other.bottomRight) &&
            (this->bottomLeft == other.bottomLeft));
}

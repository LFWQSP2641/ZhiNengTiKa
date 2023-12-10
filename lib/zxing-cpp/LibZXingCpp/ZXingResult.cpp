#include "ZXingResult.h"

QString ZXingResult::getText() const
{
    return text;
}

void ZXingResult::setText(const QString &newText)
{
    if (text == newText)
        return;
    text = newText;
}

ZXingPosition ZXingResult::getPosition() const
{
    return position;
}

void ZXingResult::setPosition(const ZXingPosition &newPosition)
{
    if (position == newPosition)
        return;
    position = newPosition;
}

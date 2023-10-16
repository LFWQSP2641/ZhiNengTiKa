#include "ZXingResult.h"

ZXingResult::ZXingResult(QObject *parent)
    : QObject{parent},
      position(new ZXingPosition(this))
{

}

QString ZXingResult::getText() const
{
    return text;
}

void ZXingResult::setText(const QString &newText)
{
    if (text == newText)
        return;
    text = newText;
    emit textChanged();
}

ZXingPosition *ZXingResult::getPosition() const
{
    return position;
}

void ZXingResult::setPosition(ZXingPosition *newPosition)
{
    if (position == newPosition)
        return;
    position = newPosition;
    emit positionChanged();
}

void ZXingResult::deleteLater()
{
    this->QObject::deleteLater();
}

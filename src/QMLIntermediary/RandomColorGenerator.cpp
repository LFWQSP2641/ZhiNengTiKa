#include "RandomColorGenerator.h"

RandomColorGenerator::RandomColorGenerator(QObject *parent)
    : QObject{parent}
{

}

QColor RandomColorGenerator::generateRandomColor()
{
    int red = QRandomGenerator::global()->bounded(256);
    int green = QRandomGenerator::global()->bounded(256);
    int blue = QRandomGenerator::global()->bounded(256);

    return QColor(red, green, blue);
}

QColor RandomColorGenerator::generateRandomBrightColor()
{
    int red = QRandomGenerator::global()->bounded(128, 256);
    int green = QRandomGenerator::global()->bounded(128, 256);
    int blue = QRandomGenerator::global()->bounded(128, 256);

    return QColor(red, green, blue);
}

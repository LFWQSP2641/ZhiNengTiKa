#include "QMLUtils.h"
#include "../StaticClass/Global.h"

QMLUtils::QMLUtils(QObject *parent)
    : QObject{parent}
{

}

QColor QMLUtils::generateRandomColor()
{
    int red = QRandomGenerator::global()->bounded(256);
    int green = QRandomGenerator::global()->bounded(256);
    int blue = QRandomGenerator::global()->bounded(256);

    return QColor(red, green, blue);
}

QColor QMLUtils::generateRandomBrightColor()
{
    int red = QRandomGenerator::global()->bounded(128, 256);
    int green = QRandomGenerator::global()->bounded(128, 256);
    int blue = QRandomGenerator::global()->bounded(128, 256);

    return QColor(red, green, blue);
}

QString QMLUtils::getResourceFilePath()
{
    return Global::dataPath().append(QStringLiteral("/Resource"));
}

#include "QMLUtils.h"

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

qreal QMLUtils::convertZAccelerationToOpacity(qreal zAcceleration)
{
    const auto transformZ(qAbs(zAcceleration) / 10);
    return 4 * transformZ * transformZ * transformZ - 6 * transformZ * transformZ + 3 * transformZ;
}

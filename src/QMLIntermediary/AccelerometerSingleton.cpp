#include "AccelerometerSingleton.h"

AccelerometerSingleton *AccelerometerSingleton::accelerometerSingleton = nullptr;

void AccelerometerSingleton::initOnce(QObject *parent)
{
    accelerometerSingleton = new AccelerometerSingleton(parent);
}

void AccelerometerSingleton::resetAccelerometerSingleton(QObject *parent)
{
    if(accelerometerSingleton != nullptr)
        accelerometerSingleton->deleteLater();
    initOnce(parent);
}

AccelerometerSingleton *AccelerometerSingleton::getAccelerometerSingleton()
{
    return accelerometerSingleton;
}

AccelerometerSingleton::AccelerometerSingleton(QObject *parent)
    : QAccelerometer{parent}
{
    setActive(true);
}

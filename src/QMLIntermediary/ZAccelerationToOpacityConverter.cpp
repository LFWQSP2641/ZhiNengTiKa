#include "ZAccelerationToOpacityConverter.h"

ZAccelerationToOpacityConverter::ZAccelerationToOpacityConverter(QObject *parent)
    : QThread{parent}
{

}

qreal ZAccelerationToOpacityConverter::getZAcceleration() const
{
    return zAcceleration;
}

void ZAccelerationToOpacityConverter::setZAcceleration(qreal newZAcceleration)
{
    if (zAcceleration == newZAcceleration)
        return;
    lock.lockForWrite();
    zAcceleration = newZAcceleration;
    read = false;
    lock.unlock();
    emit zAccelerationChanged();
}

void ZAccelerationToOpacityConverter::trySetZAcceleration(qreal newZAcceleration)
{
    if (zAcceleration == newZAcceleration)
        return;
    if(!lock.tryLockForWrite())
        return;
    zAcceleration = newZAcceleration;
    read = false;
    lock.unlock();
    emit zAccelerationChanged();
}

void ZAccelerationToOpacityConverter::start(Priority priority)
{
    canRun = true;
    this->QThread::start(priority);
}

void ZAccelerationToOpacityConverter::stop()
{
    canRun = false;
}

void ZAccelerationToOpacityConverter::wait()
{
    this->QThread::wait();
}

void ZAccelerationToOpacityConverter::run()
{
    while(canRun)
    {
        if(read)
            continue;
        lock.lockForWrite();
        read = true;
        const auto opacity(convertZAccelerationToOpacity(zAcceleration));
        emit opacityChanged(opacity);
        lock.unlock();
    }
}

qreal ZAccelerationToOpacityConverter::convertZAccelerationToOpacity(qreal zAcceleration)
{
    const auto transformZ(qAbs(zAcceleration) / 10);
    return 4 * transformZ * transformZ * transformZ - 6 * transformZ * transformZ + 3 * transformZ;
}

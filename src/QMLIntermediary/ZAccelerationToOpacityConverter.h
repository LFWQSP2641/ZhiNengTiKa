#ifndef ZACCELERATIONTOOPACITYCONVERTER_H
#define ZACCELERATIONTOOPACITYCONVERTER_H

#include <QThread>

class ZAccelerationToOpacityConverter : public QThread
{
    Q_OBJECT
public:
    explicit ZAccelerationToOpacityConverter(QObject *parent = nullptr);
    qreal getZAcceleration() const;
    void setZAcceleration(qreal newZAcceleration);

public slots:
    void trySetZAcceleration(qreal newZAcceleration);
    void start(QThread::Priority priority = InheritPriority);
    void stop();
    void wait();

signals:
    void zAccelerationChanged();
    void opacityChanged(qreal opacity);

protected:
    QReadWriteLock lock;
    volatile bool read = true;
    volatile bool canRun = true;
    qreal zAcceleration = 5;
    void run() override;
    static qreal convertZAccelerationToOpacity(qreal zAcceleration);
private:
    Q_PROPERTY(qreal zAcceleration READ getZAcceleration WRITE setZAcceleration NOTIFY zAccelerationChanged FINAL)
};

#endif // ZACCELERATIONTOOPACITYCONVERTER_H

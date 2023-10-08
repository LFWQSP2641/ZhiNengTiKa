#ifndef ZACCELERATIONTOOPACITYCONVERTER_H
#define ZACCELERATIONTOOPACITYCONVERTER_H

#include <QThread>

class ZAccelerationToOpacityConverter : public QThread
{
    Q_OBJECT
public:
    explicit ZAccelerationToOpacityConverter(QObject *parent = nullptr);

    int getInterval() const;
    void setInterval(int newInterval);

public slots:
    void start(QThread::Priority priority = InheritPriority);
    void stop();
    void wait();

signals:
    void zAccelerationChanged();
    void opacityChanged(qreal opacity);

    void intervalChanged();

protected:
    int interval = 50;
    // 十秒后如果reading还是为nullptr, 说明设备可能不支持加速度传感器, 停止线程
    const int maxTryCount = interval > 0 ? (5000 / interval) : 0;
    int tryCount = 0;
    volatile bool canRun = true;
    void run() override;
    static qreal convertZAccelerationToOpacity(qreal zAcceleration);
private:
    Q_PROPERTY(int interval READ getInterval WRITE setInterval NOTIFY intervalChanged FINAL)
};

#endif // ZACCELERATIONTOOPACITYCONVERTER_H

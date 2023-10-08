#ifndef ACCELEROMETERSINGLETON_H
#define ACCELEROMETERSINGLETON_H

class AccelerometerSingleton : public QAccelerometer
{
    Q_OBJECT
public:
    static void initOnce(QObject *parent = nullptr);
    static void resetAccelerometerSingleton(QObject *parent = nullptr);
    static AccelerometerSingleton *getAccelerometerSingleton();
protected:
    AccelerometerSingleton(QObject *parent = nullptr);
    static AccelerometerSingleton *accelerometerSingleton;
};

#endif // ACCELEROMETERSINGLETON_H

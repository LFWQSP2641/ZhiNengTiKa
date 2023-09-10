#ifndef RANDOMCOLORGENERATOR_H
#define RANDOMCOLORGENERATOR_H


class RandomColorGenerator : public QObject
{
    Q_OBJECT
public:
    explicit RandomColorGenerator(QObject *parent = nullptr);
    Q_INVOKABLE QColor generateRandomColor();
    Q_INVOKABLE QColor generateRandomBrightColor();
signals:

};

#endif // RANDOMCOLORGENERATOR_H

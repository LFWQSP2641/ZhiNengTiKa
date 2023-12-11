#ifndef QMLUTILS_H
#define QMLUTILS_H


class QMLUtils : public QObject
{
    Q_OBJECT
public:
    explicit QMLUtils(QObject *parent = nullptr);
    Q_INVOKABLE QColor generateRandomColor();
    Q_INVOKABLE QColor generateRandomBrightColor();
    Q_INVOKABLE QString getResourceFilePath();
public slots:
signals:
};

#endif // QMLUTILS_H

#ifndef ZXINGRESULT_H
#define ZXINGRESULT_H

#include <QObject>
#include "ZXingCpp_global.h"
#include "ZXingPosition.h"

class ZXINGCPP_EXPORT ZXingResult : public QObject
{
    Q_OBJECT
public:
    explicit ZXingResult(QObject *parent = nullptr);

    QString getText() const;
    void setText(const QString &newText);

    ZXingPosition *getPosition() const;
    void setPosition(ZXingPosition *newPosition);

public slots:
    void deleteLater();

protected:
    QString text;
    ZXingPosition *position;

signals:
    void textChanged();
    void positionChanged();

private:
    Q_PROPERTY(QString text READ getText WRITE setText NOTIFY textChanged FINAL)
    Q_PROPERTY(ZXingPosition *position READ getPosition WRITE setPosition NOTIFY positionChanged FINAL)
};

#endif // ZXINGRESULT_H

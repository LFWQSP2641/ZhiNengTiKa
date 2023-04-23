#ifndef SETTINGOPERATOR_H
#define SETTINGOPERATOR_H

#include <QObject>

class SettingOperator : public QObject
{
    Q_OBJECT
public:
    explicit SettingOperator(QObject *parent = nullptr);

    Q_INVOKABLE bool login(const QString &id, const QString &pw);
    Q_INVOKABLE bool logined();
    Q_INVOKABLE QStringListModel *getUserDataListModel();
public slots:
    void save();
    void logout();
    void userDataListToFirst(qsizetype index);

protected:
    QStringListModel *userDataListModel = nullptr;

signals:

};

#endif // SETTINGOPERATOR_H

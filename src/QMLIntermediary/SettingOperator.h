#ifndef SETTINGOPERATOR_H
#define SETTINGOPERATOR_H

class SettingOperator : public QObject
{
    Q_OBJECT
public:
    explicit SettingOperator(QObject *parent = nullptr);

    Q_INVOKABLE bool login(const QString &id, const QString &pw);
    Q_INVOKABLE QStringListModel *getUserDataListModel();
    Q_INVOKABLE QString getVersion();
public slots:
    void userDataListToFirst(qsizetype index);
    void deleteTemplateFile();

protected:
    QStringListModel *userDataListModel = nullptr;

signals:

};

#endif // SETTINGOPERATOR_H

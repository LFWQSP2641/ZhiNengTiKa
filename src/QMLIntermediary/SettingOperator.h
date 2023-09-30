#ifndef SETTINGOPERATOR_H
#define SETTINGOPERATOR_H

class SettingOperator : public QObject
{
    Q_OBJECT
public:
    explicit SettingOperator(QObject *parent = nullptr);

    Q_INVOKABLE QByteArray getCurrentUserAccessToken() const;
    Q_INVOKABLE QByteArray getCurrentUserAuthorization() const;
    Q_INVOKABLE QByteArray getCurrentUserClientSession() const;
    Q_INVOKABLE QByteArray getCurrentUserPassword() const;
    Q_INVOKABLE QByteArray getCurrentUserSchoolId() const;
    Q_INVOKABLE QByteArray getCurrentUserStudentId() const;
    Q_INVOKABLE QByteArray getCurrentUserUsername() const;
    Q_INVOKABLE bool isCurrentUserEmpty() const;
    Q_INVOKABLE bool isCurrentUserValid() const;

    Q_INVOKABLE bool login(const QString &id, const QString &pw);
    Q_INVOKABLE bool currentUserRelogin();
    Q_INVOKABLE QVariant getUserDataList();
    Q_INVOKABLE QString getVersion();
public slots:
    void userDataListToFirst(qsizetype index);
    void deleteTemplateFile();
signals:

};

#endif // SETTINGOPERATOR_H

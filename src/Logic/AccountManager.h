#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include "UserData.h"

class AccountManager : public QObject, public QList<UserData *>
{
    Q_OBJECT
public:
    explicit AccountManager(QObject *parent = nullptr);

    UserData *getPublicUserData() const;

    Q_INVOKABLE UserData *getCurrentUserData() const;
    Q_INVOKABLE bool isLoggedin() const;
    Q_INVOKABLE QVariant getDescriptionOfUserDatas() const;

public slots:
    void login(const QString &username, const QString &password);
    void loginUtf8(const QByteArray &username, const QByteArray &password);
    void relogin(qsizetype index = 0);
    void checkCurrentAccountValid();
    void initPublicUserData();

    void toFirst(qsizetype i);
    void removeFirst();
    void logout();

protected:
    QNetworkReply *getLoginReply(const QString &username, const QString &password);
    QNetworkReply *getLoginReplyUtf8(const QByteArray &username, const QByteArray &password);
    QHash<QNetworkReply *, QList<QByteArray>> loginHash;
    QHash<QNetworkReply *, UserData *> reloginHash;

    UserData *publicUserData;

protected slots:
    void onLoginReplyFinished();
    void onReloginReplyFinished();
    void onCheckCurrentAccountValidReplyFinished();
    void onPublicUserDataReplyFinished();

signals:
    void loginFinished(bool success, UserData *object);
    void reloginFinished(bool success, UserData *object);
    void checkCurrentAccountValidFinished(bool valid);
    void initPublicUserDataFinished(UserData *object);
    void error(const QString &msg);
private:
    Q_PROPERTY(UserData *publicUserData READ getPublicUserData CONSTANT FINAL)
};

#endif // ACCOUNTMANAGER_H

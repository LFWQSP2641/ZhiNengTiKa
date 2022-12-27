#pragma once


namespace Login
{
enum loginState
{
    Success = 200,
    SomethingIsEmpty = 400,
    SomethingIsWrong = 415,
    error = -1
};
extern QByteArray login(const QByteArray &username, const QByteArray &password);
extern loginState refreshAuthorization();
extern loginState refreshUserData();
extern loginState refreshUserData(const QByteArray &username, const QByteArray &password);
extern void refreshUserClientSession();
extern void userLogout();

extern bool userLogined;
};


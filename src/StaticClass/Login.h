#pragma once

#include "../Logic/UserData.hpp"

namespace Login
{
//enum LoginState
//{
//    Success = 200,
//    SomethingIsEmpty = 400,
//    SomethingIsWrong = 415,
//    error = -1
//};
extern UserData login(const QByteArray &username, const QByteArray &password);
};


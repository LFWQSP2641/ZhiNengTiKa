#pragma once

#include "UserData.hpp"

class UserDataList: public QList<UserData>
{
public:
    void toFirst(qsizetype i)
    {
        const auto currentUserData{ this->at(i) };
        for (; i > 0; --i)
        {
            this->begin()[i] = this->at(i - 1);
        }
        this->begin()[0] = currentUserData;
    }
};

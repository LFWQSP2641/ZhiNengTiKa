#pragma once

#include "../Logic/NetworkAccessManagerBlockable.h"

class Network
{
public:
    static void initOnce();
    static void resetNetworkManager();
    static NetworkAccessManagerBlockable *getGlobalNetworkManager();
private:
    Network() = default;
    static NetworkAccessManagerBlockable *globalManager;
};

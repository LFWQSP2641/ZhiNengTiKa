#include "Network.h"

NetworkAccessManagerBlockable *Network::globalManager;

void Network::initOnce()
{
    Network::globalManager = new NetworkAccessManagerBlockable;
}

void Network::resetNetworkManager()
{
    globalManager->deleteLater();
    initOnce();
}

NetworkAccessManagerBlockable *Network::getGlobalNetworkManager()
{
    return globalManager;
}

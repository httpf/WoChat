#include <memory>
#include "server.h"

int main()
{
    SockServer::initialize();

    std::shared_ptr<SockServer> spServer(new SockServer);
    
    return 0;
}
#pragma once
#include "Utility/Network/Network.h"

class ThreadObj;

class ClientNetWork : public Network
{
public:
    ClientNetWork();
    ~ClientNetWork();

    bool Init(ThreadObj* thread) override;
    bool ConnectServer();

    bool SendRequest(std::string data, SOCKET* socket);
    std::string Recieve(SOCKET* socket);
    bool Close();

    inline SOCKET* GetClientSocket() { return &mConnectSocket; }
    
private:
    SOCKET mConnectSocket = {};
    
};
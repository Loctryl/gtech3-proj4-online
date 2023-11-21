#pragma once
#include "Utility/RequestManager/RequestManager.h"
#include "Headers/ServerNetWork.h"

class ServerRequestManager : public RequestManager
{
public:
    ~ServerRequestManager();

    static ServerRequestManager* GetInstance();

    bool Init();

    bool ManageMessage(std::string Message, SOCKET* socket);

    bool SendToWeb(std::string request, SOCKET* socket) const;

    inline ServerNetWork* GetNetWork() { return (ServerNetWork*)mNetWork; }

private:
    static ServerRequestManager* mInstance;

    ServerRequestManager();

    bool SendRequestValidation(bool validation, SOCKET* socket) const;

    bool SendRequestWinner(int winner, SOCKET* socket) const;
};

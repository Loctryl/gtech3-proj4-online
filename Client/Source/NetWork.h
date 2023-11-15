#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include "Ressources/json.hpp"
#include "Ressources/framework.h"

using json = nlohmann::json;

#pragma comment(lib, "Ws2_32.lib")

#define PORT 6666
#define ADRESSE "127.0.0.1"

#define PACKET_SIZE 2048

class NetWork
{
public:
    ~NetWork();

    static NetWork* GetInstance();

    bool Init(HWND hWnd);

    bool SendRequest(int x, int y);
    json Recieve();
    void Close();

    inline SOCKET GetSocket() { return mConnectSocket; };
    
private:
    NetWork();

    static NetWork* mInstance;

    SOCKET mConnectSocket = {};
    HWND mHWnd;

    bool SettingSocket();
    bool CreateSocket();
    sockaddr_in SettingProtocol();
    bool ConnectServer(sockaddr_in& clientService);
};

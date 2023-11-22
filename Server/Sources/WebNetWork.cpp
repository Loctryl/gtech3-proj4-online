#include "Headers/WebNetWork.h"
#include "Headers/MessageWindow.h"

WebNetWork::WebNetWork() { }

bool WebNetWork::Init()
{
    Network::Init(mWebSocket);

    sockaddr_in service = SettingWebProtocol();

    if (!Bind(service, &mWebSocket))
        return false;

    if (!WaitWebClients())
        return false;

    WSAAsyncSelect(mWebSocket, MessageWindow::GetHWND(), WM_WEBSOCKET, FD_ACCEPT);

    return true;
}

bool WebNetWork::Bind(sockaddr_in& serviceServer, SOCKET* socket)
{
    if (bind(*socket, (SOCKADDR*)&serviceServer, sizeof(serviceServer)) == SOCKET_ERROR)// Associe l'adresse locale au socket
    {
        printf("Erreur bind() %d\n", WSAGetLastError());
        Network::CloseSocket(*socket);
        return false;
    }
    else
        return true;
}

bool WebNetWork::WaitWebClients()
{
    if (listen(mWebSocket, 1) == SOCKET_ERROR)
    {
        printf("Erreur lors de l'ecoute : %d\n", WSAGetLastError());
        Network::CloseSocket(mWebSocket);
        return false;
    }

    printf("Attente de la connexion du client Web\n");
    return true;
}

bool WebNetWork::AcceptWebClient(SOCKET* socket)
{
    *socket = accept(mWebSocket, NULL, NULL);
    if (*socket == INVALID_SOCKET)
    {
        printf("Erreur accept() web socket : %d\n", WSAGetLastError());
        return false;
    }

    printf("Client Web connecte\n");
    return true;
}

bool WebNetWork::SendRequest(std::string data, SOCKET* socket)
{
    bool result = Network::SendRequest(*socket, data);

    if (!result)
        Close();

    return result;
}

std::string WebNetWork::Recieve(SOCKET* socket)
{
    std::string result = Network::Receive(socket);

    return result;
}

bool WebNetWork::Close()
{
    bool closeSuccess = true;

    if (!Network::CloseSocket(mWebSocket))
        closeSuccess = false;

    WSACleanup();

    return closeSuccess;
}
#pragma once

#pragma comment(lib, "sfml-graphics.lib")
#pragma comment(lib, "sfml-system.lib")
#pragma comment(lib, "sfml-window.lib")

#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>

#include <windows.h>
#include <iostream>

#define WM_SOCKET WM_USER+1
#define WM_WEBSOCKET WM_USER+2

using namespace std;
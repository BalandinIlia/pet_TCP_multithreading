#include "pch.h"
#include "utils.h"

#pragma comment(lib, "ws2_32.lib")

CWSAConfig::CWSAConfig()
{
    WSADATA wsaData;
    WSAStartup(0x202, &wsaData);
}

CWSAConfig::~CWSAConfig()
{
    WSACleanup();
}

void recvAll(SOCKET id, char* buf, int len)
{
    int bytes = 0;
    while (len != 0)
    {
        bytes = recv(id, buf, len, 0);
        buf += bytes;
        len -= bytes;
        bytes = 0;
    }
}

void sendAll(SOCKET id, char* buf, int len)
{
    int bytes = 0;
    while (len != 0)
    {
        bytes = send(id, buf, len, 0);
        buf += bytes;
        len -= bytes;
        bytes = 0;
    }
}
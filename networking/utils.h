#pragma once
#include "pch.h"

class CWSAConfig
{
public:
    CWSAConfig();
    ~CWSAConfig();
};

void recvAll(SOCKET id, char* buf, int len);
void sendAll(SOCKET id, char* buf, int len);
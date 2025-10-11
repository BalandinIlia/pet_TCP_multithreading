#pragma once
#include "pch.h"

typedef unsigned long IPAddr;
typedef unsigned short TCPPort;
typedef unsigned long long number;
typedef std::lock_guard<std::mutex> LG;

extern const IPAddr addrIP;
extern const TCPPort port;

extern const int delayAlgo;
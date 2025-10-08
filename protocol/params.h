#pragma once
#include "winsock2.h"

static_assert(sizeof(unsigned long) == 4, "wrong type for IP address");
constexpr unsigned long addrIP = INADDR_LOOPBACK;

static_assert(sizeof(unsigned short) == 2, "wrong type for TCP port");
constexpr unsigned short TCPPort = 5555;

typedef unsigned long long number;
static_assert(sizeof(number) == 8, "wrong type of number type");
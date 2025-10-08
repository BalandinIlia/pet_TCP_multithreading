#include "winsock2.h"
#include "iostream"

#pragma comment(lib, "ws2_32.lib")

void runServer()
{
    WSADATA wsaData;
    WSAStartup(0x202, &wsaData);

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == INVALID_SOCKET)
        std::cerr << "socket() failed: " << WSAGetLastError() << "\n";

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    serverAddr.sin_port = htons(5555);
    bind(listenSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));

    listen(listenSocket, SOMAXCONN);

    sockaddr_in clientAddr{};
    int clientLen = sizeof(clientAddr);
    SOCKET conn = accept(listenSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientLen);

    std::cout << "Server" << std::endl;
    std::cout << "connected" << std::endl;

    uint8_t number;
    int bytes = recv(conn, reinterpret_cast<char*>(&number), 1, 0);

    std::cout << number;
    number++;

    bytes = send(conn, reinterpret_cast<char*>(&number), 1, 0);

    for (;;) {}
}

void runClient()
{
    WSADATA wsaData;
    WSAStartup(0x202, &wsaData);

    SOCKET socke = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socke == INVALID_SOCKET)
        std::cerr << "socket() failed: " << WSAGetLastError() << "\n";

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    serverAddr.sin_port = htons(5555);

    connect(socke, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));

    std::cout << "Client" << std::endl;

    uint8_t number;
    std::cin >> number;
    int bytes = send(socke, reinterpret_cast<char*>(&number), 1, 0);

    bytes = recv(socke, reinterpret_cast<char*>(&number), 1, 0);

    std::cout << number;

    for (;;) {}
}

#define SERVER
int main()
{
#ifdef SERVER
    runServer();
#else
    runClient();
#endif
    return 0;
}
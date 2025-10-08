#include "winsock2.h"
#include "iostream"
#include "../protocol/messages.h"
#include "../protocol/utils.h"

int main()
{
    CWSAConfig conf;

    std::cout << "Server" << std::endl;

    SOCKET idSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (idSocket == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << "\n";
        std::cin.get();
        return 0;
    }
        
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(addrIP);
    serverAddr.sin_port = htons(TCPPort);
    bind(idSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));

    listen(idSocket, SOMAXCONN);

    SOCKET conn = accept(idSocket, nullptr, nullptr);

    std::cerr << "Client connected" << std::endl;

    std::array<char, 9> message;
    recvAll(conn, message.data(), 9);

    const char code = message[0];
    if (MS::decodeType(code) != MS::ETypeMes::eReq)
    {
        std::cerr << "Wrong request" << "\n";
        std::cin.get();
        return 0;
    }

    std::array<char, 8> num;
    for (int i = 0; i < 8; i++)
        num[i] = message[i + 1];

    number h = MS::deserializeRequest(num);
    int count = 0;
    while (h >= 10)
    {
        h /= 10;
        count++;
    }
    std::cerr << count;
    std::cin.get();

    return 0;
}
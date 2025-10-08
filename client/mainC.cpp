#include "winsock2.h"
#include "iostream"
#include "../protocol/messages.h"
#include "../protocol/utils.h"

int main()
{
    CWSAConfig conf;

    std::cout << "Client" << std::endl;

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
    connect(idSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));

    number num = 0;
    std::cin >> num;

    std::array<char, 9> mes = MS::serializeRequest(num);

    sendAll(idSocket, mes.data(), 9);
    std::cin.get();

    return 0;
}
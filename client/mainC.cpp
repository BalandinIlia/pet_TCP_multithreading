#include "winsock2.h"
#include "iostream"
#include "thread"
#include "mutex"
#include "../shared/messages.h"
#include "../shared/utils.h"

// mutex controlling console
std::mutex mutCons;

void send(SOCKET idSocket)
{
    short id = 1;
    for (;;)
    {
        std::cin.get();
        mutCons.lock();
        number num = 0;
        std::cin >> num;
        std::cout << "Sent request id " << id << " with number " << num << std::endl;
        mutCons.unlock();
        std::array<char, 11> mes = MS::serializeRequest(num, id);
        sendAll(idSocket, mes.data(), 11);
        id++;
    }
}

void receive(SOCKET idSocket)
{
    for (;;)
    {

    }
}

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

    std::thread tSend()

    return 0;
}
